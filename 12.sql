SELECT title
FROM people
JOIN stars AS s ON people.id = s.person_id
JOIN movies AS m ON s.movie_id = m.id
WHERE name IN ('Johnny Depp', 'Helena Bonham Carter');
