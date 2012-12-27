
CREATE TABLE IF NOT EXISTS Markets
(
	id                   INTEGER AUTO_INCREMENT,
	address              TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS Users
(
	id                   INTEGER AUTO_INCREMENT PRIMARY KEY,
	login                TEXT NULL,
	passwordCrypt        TEXT NULL,
	name                 TEXT NULL
);



CREATE TABLE IF NOT EXISTS Clients
(
	id                   INTEGER AUTO_INCREMENT,
	name                 TEXT NULL,
	birthDate            DATE NULL,
	login                TEXT NULL,
	passwordHash         TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS ReturnedTickets
(
	id                   INTEGER AUTO_INCREMENT,
	id_client            INTEGER NULL,
	date                 DATE NULL,
	identifier           TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS ClientIdentifier
(
	id                   INTEGER AUTO_INCREMENT,
	id_client            INTEGER NULL,
	identifier           TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS Categories
(
	id                   INTEGER AUTO_INCREMENT PRIMARY KEY ,
	name                 TEXT NULL
);



CREATE TABLE IF NOT EXISTS Places
(
	id                   INTEGER AUTO_INCREMENT,
	title                TEXT NULL,
	address              TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS Images
(
	id                   INTEGER AUTO_INCREMENT,
	path                 TEXT NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS Actions
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



CREATE TABLE IF NOT EXISTS PlaceScheme
(
	id                   INTEGER AUTO_INCREMENT,
	seatNumber           INTEGER NULL,
	x                    FLOAT NULL,
	y                    FLOAT NULL,
	id_place             INTEGER NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS Reservations
(
	id                   INTEGER AUTO_INCREMENT,
	id_client            INTEGER NULL,
	dateTIme             DATETIME NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS ActionPriceGroups
(
	id                   INTEGERAUTO_INCREMENT,
	name                 TEXT NULL,
	price                INTEGER NULL,
	id_action            INTEGER NULL,
	color                INTEGER NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS ActionScheme
(
	id_action            INTEGER AUTO_INCREMENT,
	id_placeScheme       INTEGER NOT NULL,
	hide                 boolean DEFAULT 0,
	id_reservation       INTEGER NULL,
	id_priceGroup        INTEGER NULL,
	PRIMARY KEY (id_action)
);



CREATE TABLE IF NOT EXISTS Tickets
(
	id                   INTEGER AUTO_INCREMENT,
	id_action            INTEGER NULL,
	id_placeScheme       INTEGER NULL,
	PRIMARY KEY (id)
);



CREATE TABLE IF NOT EXISTS ClientTickets
(
	id_client            INTEGER NOT NULL,
	id_ticket            INTEGER NOT NULL,
	identifier           TEXT NULL,
	PRIMARY KEY (id_client,id_ticket)
);



CREATE TABLE IF NOT EXISTS ActionImages
(
	id_action            INTEGER NOT NULL,
	id_image             INTEGER NOT NULL,
	PRIMARY KEY (id_action,id_image)
);



CREATE TABLE IF NOT EXISTS PlaceImages
(
	id_place             INTEGER NOT NULL,
	id_image             INTEGER NOT NULL,
	PRIMARY KEY (id_place,id_image)
);



CREATE TABLE IF NOT EXISTS CategoryImage
(
	id_category          INTEGER NOT NULL,
	id_image             INTEGER NOT NULL,
	PRIMARY KEY (id_image,id_category)
);


