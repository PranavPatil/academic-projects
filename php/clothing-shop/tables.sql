
create table if not exists orders 
(orderid int(35) Primary key AUTO_INCREMENT,
 fname varchar(20),
 lname varchar(20),
 email varchar(30),
 phone1 int(4), 
 phone2 int(4), 
 phone3 int(4), 
 address1 varchar(100),
 address2 varchar(100),
 city varchar(20),
 state varchar(20),
 country varchar(20),
 zipcode int(10)
);
