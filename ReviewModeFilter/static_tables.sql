DROP TABLE IF EXISTS Feature_Category_List;
CREATE TABLE Feature_Category_List
(
	ID INTEGER NOT NULL,
	NAME VARCHAR(255) NOT NULL,
	DESCRIPTION TEXT,
	PRIMARY KEY (ID),
	UNIQUE UQ_Feature_Category_List_ID(ID)
) ;
insert into feature_category_list (ID, Name) values (0x02000000,'VEHICLES');
insert into feature_category_list (ID, Name) values (0x04000000,'TRAFFIC_SIGN');

DROP TABLE IF EXISTS Event_Status_List;
CREATE TABLE Event_Status_List
(
	ID INTEGER NOT NULL,
	NAME VARCHAR(255) NOT NULL,
	DESCRIPTION TEXT,
	PRIMARY KEY (ID),
	UNIQUE UQ_Event_Status_List_ID(ID)
);

insert into event_status_list (id, name) values (1,'UNCONFIRMED');
insert into event_status_list (id, name) values (2,'ACCEPTED');
insert into event_status_list (id, name) values (3,'ANNOTATED');
insert into event_status_list (id, name) values (4,'DECLIED');
insert into event_status_list (id, name) values (5,'MISSED');

DROP TABLE IF EXISTS Annotation_Category_List;
CREATE TABLE Annotation_Category_List
(
	ID INTEGER NOT NULL,
	NAME VARCHAR(255) NOT NULL,
	DESCRIPTION TEXT,
	PRIMARY KEY (ID),
	UNIQUE UQ_Annotation_Category_List_ID(ID)
) ;

insert into annotation_category_list (id, name) values (0x00000000,'FREE_TEXT');
insert into annotation_category_list (id, name) values (0x04000001,'TSR_ANNOT_S_MISSED');
insert into annotation_category_list (id, name) values (0x04000002,'TSR_ANNOT_S_FALSE_DETECTED');
insert into annotation_category_list (id, name) values (0x04000003,'TSR_ANNOT_S_HOST_REL');
insert into annotation_category_list (id, name) values (0x04000004,'TSR_ANNOT_S_HOST_IRRREL');
insert into annotation_category_list (id, name) values (0x04000005,'TSR_ANNOT_S_EBD_ONTRUCK');
insert into annotation_category_list (id, name) values (0x04000006,'TSR_ANNOT_A_FALSE_DETECTED');
insert into annotation_category_list (id, name) values (0x04000007,'TSR_ANNOT_A_GEN_DETECTED');
insert into annotation_category_list (id, name) values (0x04000008,'TSR_ANNOT_W_MISSED');
insert into annotation_category_list (id, name) values (0x04000009,'TSR_ANNOT_W_FALSE_DETECTED');
insert into annotation_category_list (id, name) values (0x0400000A,'TSR_ANNOT_W_HOST_REL');
insert into annotation_category_list (id, name) values (0x0400000B,'TSR_ANNOT_S_MISSED');
insert into annotation_category_list (id, name) values (0x0400000C,'TSR_ANNOT_W_HOST_IRRREL');