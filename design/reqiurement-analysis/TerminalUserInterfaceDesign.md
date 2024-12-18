# Terminal User Interface Design

The Nook: Business Progress Diagram of Norb's Bookstore

Because the final app will have a web frontend, this design script is for reference only, and many functionalities may be different in the end.

## Startup page

Dial `QUIT` to quit the software.

```text
user@host:~$ nook
 _____ _                __                  
/__   \ |__   ___    /\ \ \___   ___   /\ /\
  / /\/ '_ \ / _ \  /  \/ / _ \ / _ \ / //_/
 / /  | | | |  __/ / /\  / (_) | (_) / __ \ 
 \/   |_| |_|\___| \_\ \/ \___/ \___/\/  \/ 
                                            
(nook) Welcome to The Nook - your exclusive bookstore!
(nook) Dial QUIT to go back or exit the app.
(nook) Please login or register to continue.
(visitor) _
```

## Register and Login

Dial `QUIT` to head to the main page.

```text
(visitor) register
(nook) Username: <input> ***
(nook) This username has been occupied! Login instead?
(nook) Username: <input> ***
(nook) Password: <input> *****
(nook) Repeat Password: <input> *****
(nook) Different password from before! Please be more careful next time!
(nook) Password: <input> *****
(nook) Repeat Password: <input> *****
(nook) Registration successful!
(nook) Welcome, ***!
(customer/salesperson/shopkeeper)(***) _
```

```text
(visitor) login
(nook) Username: <input> ***
(nook) Password: <input> ***
(nook) Wrong password or username!
(nook) Username: <input> ***
(nook) Password: <input> ***
(nook) Welcome back, ***!
(customer/salesperson/shopkeeper)(***) _
```

## Data Management

Dial `QUIT` to quit the app.

```text
(customer/salesperson/shopkeeper)(***) purchase <isbn> <amount>
(nook) Sorry, the book is not found!
(nook) Sorry, the book is out of stock!
(nook) You bought <amount> <book_name>!
(nook) Enjoy reading!
(customer/salesperson/shopkeeper)(***) _
```

```text
(customer/salesperson/shopkeeper)(***) query --isbn <isbn> --title <title> --author <author> --keywords <keywords*>
(nook) Sorry, the book is not found!
(nook) Found <number> books:

┌─────┬─────────┬─────────┬────────┬──────────────────┬───────┬──────────┐
│ NUM │  ISBN   │  TITLE  │ AUTHOR │     KEYWORDS     │ STOCK │ PRICE($) │
├─────┼─────────┼─────────┼────────┼──────────────────┼───────┼──────────┤
│ 001 │ <ISBN>  │ <TITLE> │<AUTHOR>│  <KEY1,KEY2,...> │<STOCK>│ <PRICE>  │
├─────┼─────────┼─────────┼────────┼──────────────────┼───────┼──────────┤
│ 002 │ <ISBN>  │ <TITLE> │<AUTHOR>│  <KEY1,KEY2,...> │<STOCK>│ <PRICE>  │
├─────┼─────────┼─────────┼────────┼──────────────────┼───────┼──────────┤
│ 003 │ <ISBN>  │ <TITLE> │<AUTHOR>│  <KEY1,KEY2,...> │<STOCK>│ <PRICE>  │
└─────┴─────────┴─────────┴────────┴──────────────────┴───────┴──────────┘

(customer/salesperson/shopkeeper)(***) _
```

```text
(salesperson/shopkeeper)(***) restock <isbn> <amount> <price>
(nook) This is the first time <isbn> is bought. Please fill out the form:
(nook) ISBN: <isbn>
(nook) Title: <input> ***
(nook) Author: <input> ***
(nook) Keywords(separate with ,): <input> ***,***,***
(nook) Price: <input> ***
(nook) Books added to the store!
(salesperson/shopkeeper)(***) _
```

```text
(salesperson/shopkeeper)(***) edit <isbn>
(nook) Param to edit: <input> param
(nook) New value for <param>: <input> ***
(nook) Value updated for <isbn>: <param>!
(nook) Current Status:

┌─────────┬─────────┬────────┬──────────────────┬───────┬──────────┐
│  ISBN   │  TITLE  │ AUTHOR │     KEYWORDS     │ STOCK │ PRICE($) │
├─────────┼─────────┼────────┼──────────────────┼───────┼──────────┤
│ <ISBN>  │ <TITLE> │<AUTHOR>│  <KEY1,KEY2,...> │<STOCK>│ <PRICE>  │
└─────────┴─────────┴────────┴──────────────────┴───────┴──────────┘
  
(salesperson/shopkeeper)(***) _
```

```text
(salesperson/shopkeeper)(***) add <category> <username> <password>
(nook) Repeat password: <input> ***
(nook) Added new <category> <username>!
(salesperson/shopkeeper)(***) _
```

```text
(shopkeeper)(***) query restock <begin> <end>

┌─────┬────────┬────────┬─────────────┬─────────┐                                               
│ NUM │  TIME  │  ISBN  │  RESTOCKED  │  PRICE  │                                               
├─────┼────────┼────────┼─────────────┼─────────┤                                               
│ 001 │ <TIME> │ <ISBN> │ <RESTOCKED> │ <PRICE> │                                               
├─────┼────────┼────────┼─────────────┼─────────┤                                               
│ 002 │ <TIME> │ <ISBN> │ <RESTOCKED> │ <PRICE> │                                               
├─────┼────────┼────────┼─────────────┼─────────┤                                               
│ 003 │ <TIME> │ <ISBN> │ <RESTOCKED> │ <PRICE> │                                               
└─────┴────────┴────────┴─────────────┴─────────┘                                               

(shopkeeper)(***) query purchase <begin> <end>

┌─────┬────────┬────────┬─────────────┬─────────┐  
│ NUM │  TIME  │  ISBN  │  PURCHASES  │  PRICE  │  
├─────┼────────┼────────┼─────────────┼─────────┤  
│ 001 │ <TIME> │ <ISBN> │ <PURCHASES> │ <PRICE> │  
├─────┼────────┼────────┼─────────────┼─────────┤  
│ 002 │ <TIME> │ <ISBN> │ <PURCHASES> │ <PRICE> │  
├─────┼────────┼────────┼─────────────┼─────────┤  
│ 003 │ <TIME> │ <ISBN> │ <PURCHASES> │ <PRICE> │  
└─────┴────────┴────────┴─────────────┴─────────┘  
                                                      
(shopkeeper)(***) query profit <begin> <end>
(nook) From <begin> to <end>, the bookstore spent $<spending>, earned $<earning>, and made profit $<profit>.
 
(shopkeeper)(***) query worker (<begin> <end>)

┌─────┬────────┬──────────────────────────────────────┐
│ NUM │  TIME  │               ACTION                 │
├─────┼────────┼──────────────────────────────────────┤
│ 001 │ <TIME> │ add <category> <username> <password> │
├─────┼────────┼──────────────────────────────────────┤
│ 002 │ <TIME> │ restock <isbn> <amount> <price>      │
├─────┼────────┼──────────────────────────────────────┤
│ 003 │ <TIME> │ edit <isbn>                          │
└─────┴────────┴──────────────────────────────────────┘

(shopkeeper)(***) query log (<begin> <end>)

[<time>](<scope>)[INFO]    ******
[<time>](<scope>)[INFO]    ******
[<time>](<scope>)[INFO]    ******
[<time>](<scope>)[WARNING] ******
[<time>](<scope>)[INFO]    ******
[<time>](<scope>)[INFO]    ******

(shopkeeper)(***) _
```

## Exit

```text
(visitor/customer/salesperson/shopkeeper)(***) QUIT
(nook) Goodbye! See you next time!

user@host:~$ _
```
