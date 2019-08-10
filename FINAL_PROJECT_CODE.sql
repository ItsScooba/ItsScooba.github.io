USE messaging;


#Task 1 - Insert an entry into the PERSON database, using my name.
INSERT INTO messaging.person (first_name, last_name)
VALUES ("Stephen", "Frueh");



#Task 2 - Alter the PERSON database and add a new field, called middle_name.
ALTER TABLE messaging.person
ADD middle_name VARCHAR(255);



#Task 3 - Update the entry with our name from the first task to include a middle_name field value.
UPDATE messaging.person
SET middle_name = "Robert"
WHERE first_name = "Stephen";



#Task 4 - Delete a specific entry in the PERSON database, in this case, the entry specific to Diana Taurasi.
DELETE FROM messaging.person
WHERE first_name = "Diana"
AND last_name = "Taurasi";



#Task 5 - Alter the CONTACT_LIST database to add a new field called favorite, with a default value of NULL.
ALTER TABLE messaging.contact_list
ADD favorite VARCHAR(10) DEFAULT NULL;



#Task 6 - Update the new favorite field for the database entry with a contact_id equal to 1, and set it to 'y'.
UPDATE messaging.contact_list
SET favorite = 'y'
WHERE contact_id = 1;



#Task 7 - Similar to task 6, instead of setting the favorite field with a contact_id equal to 1 to 'y', we set any contact_id not equal to 1 to 'n'.
UPDATE messaging.contact_list
SET favorite = 'n'
WHERE contact_id < 1 OR contact_id > 1;



#Task 8 - Insert three new rows of data, or records, into the table, using our entry's contact_id as a value.
INSERT INTO messaging.contact_list (person_id, contact_id, favorite)
VALUES(1, 7, 'y'),
(2, 7, 'n'),
(3,7,'y');



#Task 9 - Create a new table in the database, named IMAGE, with the fields image_id, image_name, image_location, 
	and a PRIMARY KEY represented by the image_id field, that auto-increments the image_id for each new record added to the table.
CREATE TABLE image (
    image_id INT(8) NOT NULL AUTO_INCREMENT,
    image_name VARCHAR(50) NOT NULL,
    image_location VARCHAR(250) NOT NULL,
    PRIMARY KEY (image_id)
)auto_increment = 1;



#Task 10 - Create another new table, named MESSAGE_IMAGE.
CREATE TABLE message_image (
    message_id INT(8) NOT NULL,
    image_id INT(8) NOT NULL,
    PRIMARY KEY (message_id, image_id),
    FOREIGN KEY (message_id) REFERENCES message(message_id),
    FOREIGN KEY (image_id) REFERENCES image(image_id)
);



#Task 11 - Insert several new records into the IMAGE table, in this case, 5.
INSERT INTO messaging.image (image_name, image_location)
VALUES ("image_1", "image_1_location"),
("image_2", "image_2_location"),
("image_3", "image_3_location"),
("image_4", "image_4_location"),
("image_5", "image_5_location");



#Task 12 - Insert 5 values into the MESSAGE_IMAGE table as well.
INSERT INTO message_image (message_id, image_id)
VALUES (1, 3),
(1, 1),
(3, 5),
(4, 2),
(5, 4);


#Task 13 - Select all records that show the contact Michael Phelps, and the messages he has sent to other contacts.
SELECT 
    p1.first_name AS "Sender First Name",
    p1.last_name AS "Sender Last Name",
    p2.first_name AS "Receiver First Name",
    p2.last_name AS "Receiver Last Name",
    m.message_id AS "Message ID",
    m.message AS "Message",
    m.send_datetime AS "Timestamp"
FROM
    person p1,
    person p2,
    message m
WHERE
    p1.first_name = "Michael" AND
    p1.last_name = "Phelps" AND
    p1.person_id = m.sender_id AND
    p2.person_id = m.receiver_id;
    
    


#Task 14 - Count the total number of messages sent by each person to other contacts.
SELECT
    COUNT(m.sender_id) AS "Count of Messages",
    p.person_id AS "Person ID",
    p.first_name AS "First Name",
    p.last_name AS "Last Name"
FROM
    person p,
    message m
WHERE
    p.person_id = m.sender_id
GROUP BY p.person_id;




#Task 15 - Display only the first image each person has sent another contact through a message.
SELECT
    m.message_id AS "Message ID",
    m.message AS "Message",
    m.send_datetime AS "Timestamp",
    i.image_name AS "First Image Name",
    i.image_location AS "First Image Location"
FROM
    message m
INNER JOIN message_image mi ON m.message_id = mi.message_id
INNER JOIN image i ON i.image_id = mi.image_id
WHERE
    m.message_id = mi.message_id AND
    i.image_id = mi.image_id
GROUP BY m.message_id;