# shopping-app
Online shopping application facilitating buying and selling activities using C.

This project is a simple shopping program designed to facilitate the buying and selling of items, simulating an online shopping application. The application supports up to 100 users, each of whom can be a buyer, seller, or both, with a maximum of 20 items per seller.

Features
1. User Registration
    Users can register by providing a unique userID, password, address, contact number, and name.

2. User Menu
    Sell Menu:
      - Add New Item
      - Edit Stock (Replenish, Change Price, Name, Category, Description)
      - Show My Products
      - Show My Low Stock Products
    Buy Menu:
      - View all Products
      - Show all Products by a Specific Seller
      - Search Products by Category or Name
      - Add to Cart
      - Edit Cart (Remove items, Edit quantity)
    - Check Out (All items, by Seller, or Specific Item)

3. Admin Menu
    Accessible with a password ("H3LLo?")
    Show All Users
    Show All Sellers
    Show Total Sales in a Given Duration
    Show Sellers' Sales
    Show Shopaholics


4. Data Management
    Users: Stored in Users.txt, including userID, password, name, address, and contact number.
    Items: Stored in Items.txt, including productID, sellerID, item name, category, description, quantity, and price.
    Transactions: Managed in a binary file Transactions.dat.
