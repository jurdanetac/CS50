import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"

app.config['TEMPLATES_AUTO_RELOAD'] = True

Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

stocks = []

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    return render_template("index.html",
                           stocks=db.execute("SELECT * FROM stocks WHERE owner=?", session["user_id"]),
                           lookup=lookup,
                           cash=db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"])


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        shares = int(float(request.form.get("shares")))
        cash = int(db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"])

        if not (stock and shares > 0) or not (cash >= stock["price"] * shares):
            return apology("not found", 400)

        for row in db.execute("SELECT * FROM stocks"):
            if row["owner"] == session["user_id"] and row["symbol"] == stock["symbol"]:
                db.execute("UPDATE stocks SET shares=? WHERE owner=? AND symbol=?",
                           db.execute("SELECT shares FROM stocks WHERE owner=?", session["user_id"])[0]["shares"] + shares,
                           session["user_id"], stock["symbol"])
                break
        else:
            db.execute("INSERT INTO stocks (owner, symbol, name, shares) VALUES(?, ?, ?, ?)",
                       session["user_id"], stock["symbol"], stock["name"], shares)

        cash -= stock["price"] * shares
        db.execute("UPDATE users SET cash=? WHERE id=?", cash, session["user_id"])
        return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        stock = lookup(request.form.get("symbol"))

        if stock:
            return render_template("quoted.html", stock=stock)
        else:
            return apology("not found", 400)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # username and password are not blank
        if (username and password
            # Password and confirmation matches
            and password == confirmation
                # There is not a registered user with `username` already
                and not db.execute("SELECT * FROM users WHERE username = ?;", username)):

            db.execute("INSERT INTO users (username, hash) VALUES(?, ?);", username, generate_password_hash(password))

            return redirect("/login")

        else:
            return apology("Unauthorized", 400)

    # GET
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        shares = int(request.form.get("shares"))
        cash = int(db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"])

        owned_shares = db.execute("SELECT shares FROM stocks WHERE owner=? AND symbol=?", session["user_id"], stock["symbol"])

        if not owned_shares or not (stock and shares > 0 and shares <= owned_shares[0]["shares"]):
            return apology("not found", 400)

        db.execute("DELETE FROM stocks WHERE owner=? AND symbol=?", session["user_id"], stock["symbol"])
        cash += stock["price"] * shares
        db.execute("UPDATE users SET cash=? WHERE id=?", cash, session["user_id"])
        return redirect("/")

    return render_template("sell.html")
