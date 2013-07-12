-- MySQL dump 10.13  Distrib 5.1.36, for apple-darwin9.5.0 (i386)
--
-- Host: localhost    Database: dbis
-- ------------------------------------------------------
-- Server version	5.1.36

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `dbis`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `dbis` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `dbis`;

--
-- Temporary table structure for view `available_products`
--

DROP TABLE IF EXISTS `available_products`;
/*!50001 DROP VIEW IF EXISTS `available_products`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `available_products` (
  `pid` int(11),
  `cid` int(11),
  `name` varchar(45),
  `descr` tinytext,
  `price` float,
  `availability` tinyint(4),
  `deleted` tinyint(1)
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Temporary table structure for view `catalogue`
--

DROP TABLE IF EXISTS `catalogue`;
/*!50001 DROP VIEW IF EXISTS `catalogue`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `catalogue` (
  `pid` int(11),
  `cid` int(11),
  `name` varchar(45),
  `descr` tinytext,
  `price` float,
  `availability` tinyint(4),
  `deleted` tinyint(1),
  `category` varchar(45)
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `categories`
--

DROP TABLE IF EXISTS `categories`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `categories` (
  `cid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`cid`),
  UNIQUE KEY `cat_name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `configurations`
--

DROP TABLE IF EXISTS `configurations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `configurations` (
  `oid` int(11) NOT NULL,
  `pid` int(11) NOT NULL,
  PRIMARY KEY (`oid`,`pid`),
  KEY `offers` (`oid`),
  KEY `products` (`pid`),
  CONSTRAINT `offers` FOREIGN KEY (`oid`) REFERENCES `offers` (`oid`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `products` FOREIGN KEY (`pid`) REFERENCES `products` (`pid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary table structure for view `handled_orders`
--

DROP TABLE IF EXISTS `handled_orders`;
/*!50001 DROP VIEW IF EXISTS `handled_orders`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `handled_orders` (
  `oid` int(11),
  `date` datetime,
  `uid` int(11),
  `customer` varchar(41),
  `category` varchar(45),
  `product` varchar(45),
  `qty` int(11),
  `order_total` float
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `offers`
--

DROP TABLE IF EXISTS `offers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offers` (
  `oid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`oid`),
  UNIQUE KEY `nome` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `order_details`
--

DROP TABLE IF EXISTS `order_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `order_details` (
  `oid` int(11) NOT NULL,
  `pid` int(11) NOT NULL,
  `qty` int(11) DEFAULT '0',
  PRIMARY KEY (`oid`,`pid`),
  KEY `master_order` (`oid`),
  KEY `product` (`pid`),
  CONSTRAINT `master_order` FOREIGN KEY (`oid`) REFERENCES `orders` (`oid`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `product` FOREIGN KEY (`pid`) REFERENCES `products` (`pid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `orders`
--

DROP TABLE IF EXISTS `orders`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `orders` (
  `oid` int(11) NOT NULL AUTO_INCREMENT,
  `uid` int(11) NOT NULL,
  `date` datetime NOT NULL,
  `total` float DEFAULT '0',
  PRIMARY KEY (`oid`),
  KEY `users` (`uid`),
  CONSTRAINT `users` FOREIGN KEY (`uid`) REFERENCES `users` (`uid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `products`
--

DROP TABLE IF EXISTS `products`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `products` (
  `pid` int(11) NOT NULL AUTO_INCREMENT COMMENT '		',
  `cid` int(11) NOT NULL,
  `name` varchar(45) NOT NULL,
  `descr` tinytext,
  `price` float DEFAULT '0',
  `availability` tinyint(4) DEFAULT '0',
  `deleted` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`pid`),
  UNIQUE KEY `prd_name` (`name`),
  KEY `category` (`cid`),
  CONSTRAINT `category` FOREIGN KEY (`cid`) REFERENCES `categories` (`cid`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(20) NOT NULL COMMENT '	',
  `surname` varchar(20) NOT NULL,
  `login` varchar(15) NOT NULL,
  `password` varchar(45) NOT NULL,
  `address` varchar(45) NOT NULL,
  `city` varchar(15) NOT NULL,
  PRIMARY KEY (`uid`),
  UNIQUE KEY `credentials` (`login`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping routines for database 'dbis'
--
/*!50003 DROP PROCEDURE IF EXISTS `init_database` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `init_database`()
BEGIN

START TRANSACTION;

-- Emptying tables: it's mandatory follow this order to truncate the 
-- tables in order to respect FK constraints.
TRUNCATE TABLE order_details;
TRUNCATE TABLE orders;
TRUNCATE TABLE configurations;
TRUNCATE TABLE offers;
TRUNCATE TABLE users;
TRUNCATE TABLE products;
TRUNCATE TABLE categories;

-- Adding users
-- ---------------------
INSERT INTO users VALUES (0, "Ferruccio", "Vitale", "unixo", PASSWORD("ferruzzo"), "via del Sagittario, 17F", "Rome");
SELECT @uid1:=LAST_INSERT_ID();
INSERT INTO users VALUES (0, "Alessandro", "Aldini", "aaldini", PASSWORD("aldini"), "via Saffi, 42", "Urbino");
SELECT @uid2:=LAST_INSERT_ID();
INSERT INTO users VALUES (0, "Erika", "Pigliapoco", "md", PASSWORD("manager"), "via Saffi, 42", "Urbino");
SELECT @uid3:=LAST_INSERT_ID();
INSERT INTO users VALUES (0, "Alessandro", "Bogliolo", "boglio", PASSWORD("derprofessor"), "via Saffi, 42", "Urbino");
SELECT @uid4:=LAST_INSERT_ID();

-- Adding categories
-- ---------------------
INSERT INTO categories VALUES (0, "Notebook");
SELECT @cid1:=LAST_INSERT_ID();
INSERT INTO categories VALUES (0, "Monitor");
SELECT @cid2:=LAST_INSERT_ID();
INSERT INTO categories VALUES (0, "Keyboards");
SELECT @cid3:=LAST_INSERT_ID();
INSERT INTO categories VALUES (0, "PCCARD");
SELECT @cid4:=LAST_INSERT_ID();
INSERT INTO categories VALUES (0, "Hard disk");
SELECT @cid5:=LAST_INSERT_ID();
INSERT INTO categories VALUES (0, "Computer Desktop");
SELECT @cid6:=LAST_INSERT_ID();
INSERT INTO categories VALUES (0, "Audio");
SELECT @cid7:=LAST_INSERT_ID();

-- ADDING PRODUCTS
-- ---------------------
INSERT INTO products VALUES (0, @cid1, "Apple MacBook Pro", "Monitor 15 inches + HDD 250Gb solid state", 2150, 10, 0);
INSERT INTO products VALUES (0, @cid1, "Apple iMac 24", "Desktop monitor 24 inches + HDD 640Gb", 1200, 2, 0);
SELECT @pid1:=LAST_INSERT_ID();
INSERT INTO products VALUES (0, @cid2, "External monitor Acer", "Width 32 inches", 800, 1, 0);
SELECT @pid2:=LAST_INSERT_ID();
INSERT INTO products VALUES (0, @cid2, "Monitor LCD Full HD", "Resolution 1080p", 1230, 23, 0);
INSERT INTO products VALUES (0, @cid3, "Italian keyboard", "102 keys", 15, 1, 0);
SELECT @pid4:=LAST_INSERT_ID();
INSERT INTO products VALUES (0, @cid3, "Bluetooth keyboard", null, 45, 10, 0);
SELECT @pid3:=LAST_INSERT_ID();
INSERT INTO products VALUES (0, @cid4, "Wireless card 811.2e", "Compatible with Windows and Macos", 123, 0, 0);
INSERT INTO products VALUES (0, @cid4, "TV Card", "Transform your computer in a portable TV", 243, 10, 0);
SELECT @pid5:=LAST_INSERT_ID();
INSERT INTO products VALUES (0, @cid4, "NETGEAR WG511 Wireless-G", "PC Card Network adapter", 30, 5, 0);
SELECT @pid6:=LAST_INSERT_ID();
INSERT INTO products VALUES (0, @cid5, "HD esterno 1.5Tb", "Supporto USB1 & 2", 160, 10, 0);
SELECT @pid7:=LAST_INSERT_ID();
INSERT INTO products VALUES (0, @cid6, "PACKARD BELL iMax mini", "HD 160GB - RAM 1GB", 249, 11, 0);
SELECT @pid8:=LAST_INSERT_ID();
INSERT INTO products VALUES (0, @cid6, "FUJITSU-SIEMENS Amilo Li 3740", "HD 160GB - RAM 1GB", 249, 11, 0);
INSERT INTO products VALUES (0, @cid6, "HP Pavilion Elite m9441.it", "Intel Atom 230 (1.60GHz, 512KB L2, 533MHz FSB)", 369, 1, 0);
INSERT INTO products VALUES (0, @cid7, "LOGITECH LS11", "Speaker system 2.0 for Pc", 19.9, 3, 0);
SELECT @pid9:=LAST_INSERT_ID();
INSERT INTO products VALUES (0, @cid7, "HERCULES XPS 2.0 Lounge", "Speaker system 2.0 - 10W", 34.9, 7, 0);
SELECT @pid10:=LAST_INSERT_ID();

-- ADDING OFFERS
INSERT INTO offers VALUES (0, "Total Discount Offer");
SELECT @coid1:=LAST_INSERT_ID();
INSERT INTO offers VALUES (0, "Full optional Offer");
SELECT @coid2:=LAST_INSERT_ID();
INSERT INTO offers VALUES (0, "Desktop 4 everyone");
SELECT @coid3:=LAST_INSERT_ID();

-- ADDING CONFIGURATIONS
INSERT INTO configurations VALUES (@coid1, @pid2), (@coid1, @pid3), (@coid1, @pid4), (@coid2, @pid1), (@coid2, @pid3), (@coid2, @pid4), (@coid2, @pid5), (@coid2, @pid6), (@coid2, @pid7), (@coid3, @pid2), (@coid3, @pid3), (@coid3, @pid8);

-- ADDING ORDERS
-- ORDERS USER #1
INSERT INTO orders VALUES (0, @uid1, NOW(), 800);
SELECT @oid1:=LAST_INSERT_ID();
INSERT INTO order_details VALUES (@oid1, @pid2, 1), (@oid1, @pid8, 1);
-- ORDERS USER #2
INSERT INTO orders VALUES (0, @uid2, DATE_SUB(NOW(), INTERVAL 1 MONTH), 1515);
SELECT @oid2:=LAST_INSERT_ID();
INSERT INTO order_details VALUES (@oid2, @pid1, 1), (@oid2, @pid2, 1), (@oid2, @pid4, 1);
-- orders USER #3
INSERT INTO orders VALUES (0, @uid3, DATE_SUB(NOW(), INTERVAL 35 DAY), 2190);
SELECT @oid3:=LAST_INSERT_ID();
INSERT INTO order_details VALUES (@oid3, @pid1, 1), (@oid3, @pid3, 1),  (@oid3, @pid6, 1);
-- orders USER #4
INSERT INTO orders VALUES (0, @uid4, DATE_SUB(NOW(), INTERVAL 3 MONTH), 2300);
SELECT @oid4:=LAST_INSERT_ID();
INSERT INTO order_details VALUES (@oid4, @pid3, 1);
INSERT INTO order_details VALUES (@oid4, @pid7, 1);
INSERT INTO order_details VALUES (@oid4, @pid8, 1);
INSERT INTO order_details VALUES (@oid4, @pid10, 1);
-- orders USER #1 (bis)
INSERT INTO orders VALUES (0, @uid1, DATE_SUB(NOW(), INTERVAL 6 MONTH), 830);
SELECT @oid5:=LAST_INSERT_ID();
INSERT INTO order_details VALUES (@oid5, @pid3, 1), (@oid5, @pid9, 1);

COMMIT;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `offers_by_product` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `offers_by_product`(IN p_pid INT)
BEGIN

DECLARE offer_ID    INT;
DECLARE offer_name  CHAR(45);
DECLARE offer_price INT default 0;
DECLARE done        INT default 0;

DECLARE c1 CURSOR FOR SELECT DISTINCT oid FROM configurations WHERE pid = p_pid;
DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
OPEN C1;

FETCH c1 INTO offer_ID;
myloop: WHILE NOT done DO
            -- For each offer that contains the selected product, I consider
            -- its name, the list of the products composing it and the total price.
            SELECT name INTO offer_name FROM offers WHERE oid = offer_ID;
            SELECT SUM(P.price) INTO offer_price FROM products P, configurations C 
                                 WHERE P.pid = C.pid AND C.oid = offer_ID;
            SELECT offer_name, offer_price;

            SELECT P.*, CA.name AS category  
            FROM products P, configurations C, categories CA 
            WHERE C.pid = P.pid AND C.oid = offer_ID AND P.cid = CA.cid;
            -- Fetch next row from the cursor
            FETCH c1 INTO offer_ID;
       END WHILE myloop;

CLOSE c1;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `product_delete` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_AUTO_CREATE_USER' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `product_delete`(IN p_pid INT)
BEGIN

DECLARE num_rec INT;

START TRANSACTION;

SELECT COUNT(*) INTO num_rec FROM order_details WHERE pid = p_pid;
IF num_rec > 0 THEN
  UPDATE products SET deleted = 1 WHERE pid = p_pid;
ELSE
  DELETE FROM products WHERE pid = p_pid;
END IF;

COMMIT;

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Current Database: `dbis`
--

USE `dbis`;

--
-- Final view structure for view `available_products`
--

/*!50001 DROP TABLE `available_products`*/;
/*!50001 DROP VIEW IF EXISTS `available_products`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `available_products` AS select `products`.`pid` AS `pid`,`products`.`cid` AS `cid`,`products`.`name` AS `name`,`products`.`descr` AS `descr`,`products`.`price` AS `price`,`products`.`availability` AS `availability`,`products`.`deleted` AS `deleted` from `products` where ((`products`.`deleted` = 0) and (`products`.`availability` > 0)) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `catalogue`
--

/*!50001 DROP TABLE `catalogue`*/;
/*!50001 DROP VIEW IF EXISTS `catalogue`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `catalogue` AS select `p`.`pid` AS `pid`,`p`.`cid` AS `cid`,`p`.`name` AS `name`,`p`.`descr` AS `descr`,`p`.`price` AS `price`,`p`.`availability` AS `availability`,`p`.`deleted` AS `deleted`,`c`.`name` AS `category` from (`products` `p` join `categories` `c` on((`p`.`cid` = `c`.`cid`))) where ((`p`.`availability` > 0) and (`p`.`deleted` = 0)) order by `p`.`pid` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `handled_orders`
--

/*!50001 DROP TABLE `handled_orders`*/;
/*!50001 DROP VIEW IF EXISTS `handled_orders`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `handled_orders` AS select `O`.`oid` AS `oid`,`O`.`date` AS `date`,`U`.`uid` AS `uid`,concat(`U`.`name`,' ',`U`.`surname`) AS `customer`,`CA`.`name` AS `category`,`P`.`name` AS `product`,`C`.`qty` AS `qty`,`O`.`total` AS `order_total` from ((((`orders` `O` join `order_details` `C`) join `products` `P`) join `categories` `CA`) join `users` `U`) where ((`O`.`oid` = `C`.`oid`) and (`C`.`pid` = `P`.`pid`) and (`P`.`cid` = `CA`.`cid`) and (`O`.`uid` = `U`.`uid`)) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2010-01-20 18:31:30
