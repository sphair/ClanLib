BEGIN TRANSACTION;

CREATE TABLE Users (
	UserId INTEGER PRIMARY KEY,
	UserName TEXT NOT NULL,
	Password TEXT NOT NULL,
	RealName TEXT NOT NULL,
	CreateDate DATETIME DEFAULT CURRENT_TIMESTAMP,
	LastLoginDate DATETIME DEFAULT NULL
);

INSERT INTO Users (UserId, UserName, Password, RealName) VALUES (1, 'Don', 'thedon', 'John Jones');
INSERT INTO Users (UserId, UserName, Password, RealName) VALUES (2, 'Blondie', 'eek', 'Mary Smith');

CREATE TABLE Characters (
	CharacterId INTEGER PRIMARY KEY,
	UserId INTEGER NOT NULL,
	Name TEXT NOT NULL
);

INSERT INTO Characters (CharacterId, UserId, Name) VALUES (1, 1, 'TheDon');
INSERT INTO Characters (CharacterId, UserId, Name) VALUES (2, 1, 'Hedion');
INSERT INTO Characters (CharacterId, UserId, Name) VALUES (3, 1, 'Epo');
INSERT INTO Characters (CharacterId, UserId, Name) VALUES (4, 2, 'Blondie');

COMMIT;
