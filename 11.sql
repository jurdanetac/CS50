SELECT title
FROM people
JOIN stars AS s ON people.id = s.person_id
JOIN movies AS m ON s.movie_id = m.id
JOIN ratings AS r ON m.id = r.movie_id
WHERE name = 'Chadwick Boseman'
ORDER BY rating DESC
LIMIT 5;
