-- phpMyAdmin SQL Dump
-- version 4.6.6deb4+deb9u2
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Creato il: Gen 21, 2021 alle 13:41
-- Versione del server: 10.1.47-MariaDB-0+deb9u1
-- Versione PHP: 7.0.33-38+0~20210112.44+debian9~1.gbpab637c

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `ardutrackerdb`
--

-- --------------------------------------------------------

--
-- Struttura della tabella `tracking_log`
--

CREATE TABLE `tracking_log` (
  `my_id` varchar(64) NOT NULL,
  `friend_id` varchar(64) NOT NULL,
  `seen_millis` bigint(20) NOT NULL,
  `seen_time` bigint(20) NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=ARCHIVE DEFAULT CHARSET=utf8;

--
-- Dump dei dati per la tabella `tracking_log`
--

INSERT INTO `tracking_log` (`my_id`, `friend_id`, `seen_millis`, `seen_time`, `created_at`) VALUES
('IRON MAN', 'THOR', 69, '1614019056', '2021-01-05 21:10:49'),
('IRON MAN', 'THOR', 2147483647, '1614019056', '2021-01-13 12:12:23'),
('IRON MAN', 'THOR', 2147483647, '1614019056', '2021-01-13 12:15:41'),
('THOR', 'IRON MAN', 2147483647, '1614019056', '2021-01-13 12:23:08'),
('IRON MAN', 'HULK', 2147483647, '1614019056', '2021-01-14 18:51:45');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
