SELECT DISTINCT name
FROM people
JOIN stars AS s ON people.id = s.person_id
JOIN movies AS m ON s.movie_id = m.id;
WHERE title IN
SELECT title
FROM people
JOIN stars AS s ON people.id = s.person_id
JOIN movies AS m ON s.movie_id = m.id
WHERE name = 'Kevin Bacon' AND birth = 1958
AND name != 'Kevin Bacon' AND birth != 1958;
