
CREATE TABLE Markets
(
	id                   INTEGER AUTO_INCREMENT,
	address              TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE Users
(
	id                   INTEGER AUTO_INCREMENT,
	login                TEXT NULL,
	passwordHash         TEXT NULL,
	name                 TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE Clients
(
	id                   INTEGER AUTO_INCREMENT,
	name                 TEXT NULL,
	birthDate            DATE NULL,
	login                TEXT NULL,
	passwordHash         TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE ReturnedTickets
(
	id                   INTEGER AUTO_INCREMENT,
	id_client            INTEGER NULL,
	date                 DATE NULL,
	identifier           TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE ClientIdentifier
(
	id                   INTEGER AUTO_INCREMENT,
	id_client            INTEGER NULL,
	identifier           TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE Categories
(
	id                   INTEGER AUTO_INCREMENT,
	name                 TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE Places
(
	id                   INTEGER AUTO_INCREMENT,
	title                TEXT NULL,
	address              TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE Images
(
	id                   INTEGER AUTO_INCREMENT,
	path                 TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE Actions
(
	id                   INTEGER AUTO_INCREMENT,
	title                TEXT NULL,
	dateTime             DATETIME NULL,
	id_category          INTEGER NULL,
	id_place             INTEGER NULL,
	state                INTEGER NULL,
	description          TEXT NULL,
	id_background        INTEGER NULL,
	PRIMARY KEY (id)
);



CREATE TABLE PlaceScheme
(
	id                   INTEGER AUTO_INCREMENT,
	seatNumber           INTEGER NULL,
	x                    FLOAT NULL,
	y                    FLOAT NULL,
	id_place             INTEGER NULL,
	PRIMARY KEY (id)
);



CREATE TABLE Reservations
(
	id                   INTEGER AUTO_INCREMENT,
	id_client            INTEGER NULL,
	dateTIme             DATETIME NULL,
	PRIMARY KEY (id)
);



CREATE TABLE ActionPriceGroups
(
	id                   INTEGERAUTO_INCREMENT,
	name                 TEXT NULL,
	price                INTEGER NULL,
	id_action            INTEGER NULL,
	color                INTEGER NULL,
	PRIMARY KEY (id)
);



CREATE TABLE ActionScheme
(
	id_action            INTEGER AUTO_INCREMENT,
	id_placeScheme       INTEGER NOT NULL,
	hide                 boolean DEFAULT 0,
	id_reservation       INTEGER NULL,
	id_priceGroup        INTEGER NULL,
	PRIMARY KEY (id_action)
);



CREATE TABLE Tickets
(
	id                   INTEGER AUTO_INCREMENT,
	id_action            INTEGER NULL,
	id_placeScheme       INTEGER NULL,
	PRIMARY KEY (id)
);



CREATE TABLE ClientTickets
(
	id_client            INTEGER NOT NULL,
	id_ticket            INTEGER NOT NULL,
	identifier           TEXT NULL,
	PRIMARY KEY (id_client,id_ticket)
);



CREATE TABLE ActionImages
(
	id_action            INTEGER NOT NULL,
	id_image             INTEGER NOT NULL,
	PRIMARY KEY (id_action,id_image)
);



CREATE TABLE PlaceImages
(
	id_place             INTEGER NOT NULL,
	id_image             INTEGER NOT NULL,
	PRIMARY KEY (id_place,id_image)
);



CREATE TABLE CategoryImage
(
	id_category          INTEGER NOT NULL,
	id_image             INTEGER NOT NULL,
	PRIMARY KEY (id_image,id_category)
);


