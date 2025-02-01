-- Keep a log of any SQL queries you execute as you solve the mystery.

.tables -- Get an overview over the data I have

.schema crime_scene_reports

-- Find description of crime scene
sqlite> SELECT description
   ...> FROM crime_scene_reports
   ...> WHERE month = 7 AND day = 28
   ...> AND street = 'Humphrey Street';
-- -> 10:15AM in bakery (3 witnesses)

-- Learn from witnesses
SELECT transcript,name FROM interviews WHERE year = 2024 AND month = 07 AND day = 28;
--> 
+--------------------------------------------------------------+---------+
|                          transcript                          |  name   |
+--------------------------------------------------------------+---------+
| “Ah,” said he, “I forgot that I had not seen you for some we | Jose    |
| eks. It is a little souvenir from the King of Bohemia in ret |         |
| urn for my assistance in the case of the Irene Adler papers. |         |
| ”                                                            |         |
+--------------------------------------------------------------+---------+
| “I suppose,” said Holmes, “that when Mr. Windibank came back | Eugene  |
|  from France he was very annoyed at your having gone to the  |         |
| ball.”                                                       |         |
+--------------------------------------------------------------+---------+
| “You had my note?” he asked with a deep harsh voice and a st | Barbara |
| rongly marked German accent. “I told you that I would call.” |         |
|  He looked from one to the other of us, as if uncertain whic |         |
| h to address.                                                |         |
+--------------------------------------------------------------+---------+
| Sometime within ten minutes of the theft, I saw the thief ge | Ruth    |
| t into a car in the bakery parking lot and drive away. If yo |         |
| u have security footage from the bakery parking lot, you mig |         |
| ht want to look for cars that left the parking lot in that t |         |
| ime frame.                                                   |         |
+--------------------------------------------------------------+---------+
| I don't know the thief's name, but it was someone I recogniz | Eugene  |
| ed. Earlier this morning, before I arrived at Emma''s bakery, |         |
|  I was walking by the ATM on Leggett Street and saw the thie |         |
| f there withdrawing some money.                              |         |
+--------------------------------------------------------------+---------+
| As the thief was leaving the bakery, they called someone who | Raymond |
|  talked to them for less than a minute. In the call, I heard |         |
|  the thief say that they were planning to take the earliest  |         |
| flight out of Fiftyville tomorrow. The thief then asked the  |         |
| person on the other end of the phone to purchase the flight  |         |
| ticket.                                                      |         |
+--------------------------------------------------------------+---------+
| Our neighboring courthouse has a very annoying rooster that  | Lily    |
| crows loudly at 6am every day. My sons Robert and Patrick to |         |
| ok the rooster to a city far, far away, so it may never both |         |
| er us again. My sons have successfully arrived in Paris.     |         |
+--------------------------------------------------------------+---------+

--> thief left >= 10 min after theft
--> Thief withdrew money earlier that day
--> Thief had call for < 1 min between theft and leaving
--> Thief + Accomplice left Fiftyville with earliest flight on next day
--> Accomplice bought flight tickets 

-- Learn about bakery
sqlite> .schema bakery_security_logs
-- ->

-- Get license plates that left between 10:15 and 10:25
SELECT license_plate, activity, id from bakery_security_logs WHERE year = 2024 AND month = 07 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 ORDER BY hour, minute;

+---------------+----------+-----+
| license_plate | activity | id  |
+---------------+----------+-----+
| 5P2BI95       | exit     | 260 |
| 94KL13X       | exit     | 261 |
| 6P58WS2       | exit     | 262 |
| 4328GD8       | exit     | 263 |
| G412CB7       | exit     | 264 |
| L93JTIZ       | exit     | 265 |
| 322W7JE       | exit     | 266 |
| 0NTHK55       | exit     | 267 |
+---------------+----------+-----+

-- Get account number of everyone who withdrew money before theft

SELECT account_number FROM atm_transactions WHERE year = 2024 AND month = 07 AND day = 28 AND atm_location = 'Leggett Street' and transaction_type = 'withdraw';
+----------------+
| account_number |
+----------------+
| 28500762       |
| 28296815       |
| 76054385       |
| 49610011       |
| 16153065       |
| 25506511       |
| 81061156       |
| 26013199       |
+----------------+

-- Get short phone calls on that day
SELECT caller, receiver FROM phone_calls WHERE year = 2024 AND month = 07 AND day = 28 AND duration < 60;
+----------------+----------------+
|     caller     |    receiver    |
+----------------+----------------+
| (130) 555-0289 | (996) 555-8899 |
| (499) 555-9472 | (892) 555-8872 |
| (367) 555-5533 | (375) 555-8161 |
| (499) 555-9472 | (717) 555-1342 |
| (286) 555-6063 | (676) 555-6554 |
| (770) 555-1861 | (725) 555-3243 |
| (031) 555-6622 | (910) 555-3251 |
| (826) 555-1652 | (066) 555-9701 |
| (338) 555-6650 | (704) 555-2131 |
+----------------+----------------+

-- Get Fiftyville airpord ID

SELECT * FROM airports where city = 'Fiftyville';
--> 8

-- Get earliest flight that left on next day
SELECT * FROM flights WHERE year = 2024 AND month = 07 AND day = 29 ORDER BY hour, minute LIMIT 1;
+----+-------------------+------------------------+------+-------+-----+------+--------+
| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+----+-------------------+------------------------+------+-------+-----+------+--------+
| 36 | 8                 | 4                      | 2024 | 7     | 29  | 8    | 20     |
+----+-------------------+------------------------+------+-------+-----+------+--------+

-- Get passengers in that flight
SELECT * FROM passengers where flight_id = 36;
+-----------+-----------------+------+
| flight_id | passport_number | seat |
+-----------+-----------------+------+
| 36        | 7214083635      | 2A   |
| 36        | 1695452385      | 3B   |
| 36        | 5773159633      | 4A   |
| 36        | 1540955065      | 5C   |
| 36        | 8294398571      | 6C   |
| 36        | 1988161715      | 6D   |
| 36        | 9878712108      | 7A   |
| 36        | 8496433585      | 7B   |
+-----------+-----------------+------+

-- Find thief

Select name FROM people 
WHERE passport_number IN 
    (SELECT passport_number FROM passengers where flight_id = 36) 
    AND license_plate IN 
    (SELECT license_plate from bakery_security_logs WHERE year = 2024 AND month = 07 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25)
    AND (phone_number IN
    (SELECT receiver FROM phone_calls WHERE year = 2024 AND month = 07 AND day = 28 AND duration < 60)
    OR phone_number IN
    (SELECT caller FROM phone_calls WHERE year = 2024 AND month = 07 AND day = 28 AND duration < 60)
    )
    AND id IN (SELECT person_id FROM bank_accounts WHERE account_number IN
    (SELECT account_number FROM atm_transactions WHERE year = 2024 AND month = 07 AND day = 28 AND atm_location = 'Leggett Street' and transaction_type = 'withdraw')
    )
    ;

-- > it was Bruce!!

-- Accomplice
Select name FROM people 
WHERE phone_number IN
    (SELECT receiver FROM phone_calls WHERE year = 2024 AND month = 07 AND day = 28 AND duration < 60 and caller IN (SELECT phone_number FROM people WHERE name = 'Bruce'));

--> together with Robin!

-- Get airport (destination 4)
SELECT city FROM airports where id = 4;
+---------------+
|     city      |
+---------------+
| New York City |
+---------------+