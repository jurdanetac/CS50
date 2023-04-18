SELECT DISTINCT name
FROM people
JOIN directors AS d ON people.id = d.person_id
JOIN movies AS m ON d.movie_id = m.id
JOIN ratings as r ON m.id = r.movie_id
WHERE rating >= 9.0;
