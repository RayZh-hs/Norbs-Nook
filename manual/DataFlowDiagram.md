# Data Flow Diagram

```text
               ┌───────────┐                                                 
               │Login      │                                                 
               │           │ write   ┌────────────┐                          
               │Register   ├────────►│account_list│                          
               │           │ read    └─────┬──────┘                          
               │New Account├───────────────┤                                 
               └───────────┘               ▼                                 
                     ▲              ┌───────────────┐                        
                     └──────────────┤current_account│                        
                     change account └──────┬────────┘                        
                                           │                                 
┌──────────────┐──────────────┐            │        ┌──────┐                 
│              │book_storage  │            ▼        │Logger├───► save to disk
│   std::map   │              │ write ┌─────────┐   └─▲─┬──┘                 
│              │worker_history│◄──────┤ Perform ├─────┘ │                    
├──────────────┤              │       │ Action  │       ▼                    
│              │action_history│       └─────────┘     ┌───┐                  
│  BlockChain  │              ├──────────────────────►│GUI│                  
│              │profit_history│ read and print        └───┘                  
└─────┬─▲──────┘──────────────┘                                              
      │ │ use                                                                
┌─────▼─┴─────────┐                                                          
│                 │───► save to disk                                         
│  FileInterface  │                                                          
│                 │◄─── load from disk                                       
└─────────────────┘                                                          
```