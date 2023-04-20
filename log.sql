-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Description of the duck theft
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28
AND street = 'Humphrey Street';

-- Transcript of interviews conducted
SELECT * FROM interviews
WHERE month = 7 AND day = 28 AND id > 160 AND id < 164;

-- Record with vehicle plates of entrance and exit of bakery
SELECT minute, activity,
(SELECT name FROM people WHERE people.license_plate = bakery_security_logs.license_plate) as LP
FROM bakery_security_logs
WHERE month = 7 AND day = 28
AND hour = 10 AND minute > 10;

-- Withdrawals made the same day of the robbery
SELECT
people.name,
amount
FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number
JOIN people ON bank_accounts.person_id = people.id
WHERE month = 7
AND day = 28
AND transaction_type = 'withdraw'
AND atm_location = 'Leggett Street';

--
--SELECT name, bakery_security_logs.license_plate, hour, minute, activity FROM people JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
--WHERE hour = 10 AND minute > 10;

--
--SELECT * FROM people JOIN atm_transactions ON people.account_number = bakery_security_logs.account_number
--WHERE month = 7 AND day = 28 AND transaction_type = 'withdraw'
--AND atm_location = 'Leggett Street';

SELECT name, (SELECT name FROM people WHERE phone_number = receiver) as r, passport_number, license_plate FROM phone_calls JOIN people ON phone_calls.caller = people.phone_number
WHERE month = 7 AND day = 28 AND duration < 100;

SELECT
id,
(SELECT city FROM airports WHERE id = origin_airport_id) as F,
(SELECT city FROM airports WHERE id = destination_airport_id) as D,  
hour, minute
FROM flights WHERE day = 29 AND month = 7 AND F = 'Fiftyville'
ORDER BY hour ASC;

--SELECT
--(SELECT name FROM people WHERE people.passport_number = passengers.passport_number) as P FROM passengers WHERE flight_id =  36;

-- SELECT account_number as AN, (SELECT name FROM people WHERE people.id = bank_accounts.person_id) as P FROM bank_accounts;
