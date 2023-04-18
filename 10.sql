SELECT DISTINCT name
FROM people
JOIN stars AS s ON people.id = s.person_id
JOIN movies AS m ON s.movie_id = m.id
JOIN ratings as r ON m.id = r.movie_id
WHERE rating >= 9.0;
