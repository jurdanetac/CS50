SELECT name FROM people
    WHERE id IN 
    (SELECT person_id FROM stars
        WHERE movie_id IN
    (SELECT id FROM movies
        WHERE name = 'Kevin Bacon' AND birth = 1958))
AND name != 'Kevin Bacon';
