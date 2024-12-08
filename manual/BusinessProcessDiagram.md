# Business Process Diagram

```text
 The Nook: Business Progress Diagram of Norb's Bookstore      
                                                              
┌──────LOGIN RELATED──────────────────────────────────┐       
│                                                     │       
│   ┌─────────┐                                       │       
│   │ VISITOR ├──────────┐◄───────────────────────────┼──────┐
│   └─────────┘          │                            │      │
│     mod = 0            │                            │      │
│     │                Login                          │      │
│     │                  │                            │      │
│     │      ┌───────────┼─────────────────┐          │      │
│  Register  │           │                 │          │      │
│     │      │           │                 │          │      │
│     ▼      ▼           ▼                 ▼          │      │
│   ┌──────────┐  ┌─────────────┐   ┌────────────┐    │      │
│   │ CUSTOMER │  │ SALESPERSON │   │ SHOPKEEPER │    │      │
│   └──────────┘  └─────────────┘   └────────────┘    │      │
│                                                     │      │
└──────────────────────┬──────────────────────────────┘      │
                       ▼                                     │
                 ┌───────────┐   Log in again                │
                 │ USER MODE ├───────────────────────────────┘
                 └─────┬─────┘                                
                       ▼                                      
     ┌────────────────────────────────────────────┐           
     │┌─────────────────────────────┐             │           
     ││┌──────────┐                 │             │           
     │││ CUSTOMER │    SALESPERSON  │  SHOPKEEPER │           
     ││└────┬─────┘                 │             │           
     │└─────┼───────────────┬───────┘             │           
     └──────┼───────────────┼──────────────┬──────┘           
            ▼               ▼              ▼                  
       + Purchase Book  + Restock      + Query Restock        
       + Query Book     + Write Info   + Query Purchase       
                        + Edit Info    + Query Income         
                        + Create User  + View Work Report     
                                       + View Log             
```