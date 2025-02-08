import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    cash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    stocks = {}
    total = cash
    for t in transactions:
        sym = t["symbol"]
        if sym in stocks:
            stocks[sym]["shares"] += t["amount"]
        else:
            price = lookup(sym)["price"]
            stocks[sym] = {"shares": t["amount"], "price": price}
        stocks[sym]["total"] = stocks[sym]["shares"] * stocks[sym]["price"]
    for stock in stocks:   
        total += stocks[stock]["total"]
    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        
        # Check user errors
        sym = lookup(request.form.get("symbol"))
        if not (sym):
            return apology("stock doesn't exist")
        n = int(request.form.get("number"))
        if (n <= 0):
            return apology("Number has to be positive int")    
        cash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        # print(cash, "<", sym["price"], "*", n)
        if (cash < sym["price"] * n):
            return apology("Not enough cash")
        
        # Register transaction
        db.execute("INSERT INTO transactions (symbol, price, amount, user_id) VALUES (?,?,?,?)", 
                   sym["symbol"], 
                   sym["price"],
                   n,
                   session["user_id"])
        
        # Subtract cash
        db.execute("UPDATE users SET cash = (cash - ?) WHERE id = ?", (sym["price"] * n) , session["user_id"])
        
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY date", session["user_id"])
    return render_template("history.html", t=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        print(rows[0]["hash"], request.form.get("password"))
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        
        sym = request.form.get("symbol")
        if (lookup(sym)):
            return render_template("quote.html", res=lookup(sym))    
        else:
            return apology("stock doesn't exist")

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not (pas := request.form.get("password")):
            return apology("must provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("please confirm your password", 400)

        elif not (pas == request.form.get("confirmation")):
            return apology("confirmation doesn't match", 400)

        # Add user to database
        print("INSERT INTO users (username, hash) VALUES (?,?)", request.form.get("username"), generate_password_hash(request.form.get("password")))
        try:
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?,?)", request.form.get("username"), generate_password_hash(request.form.get("password"))
            )
        except:
            return apology("User already exists", 400)

        # Query database for username
        id = db.execute(
            "SELECT id FROM users WHERE username = ?", request.form.get("username")
        )
        # Remember which user has logged in
        session["user_id"] = id[0]["id"]
        
        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        
        # Check user errors
        sym = lookup(request.form.get("symbol"))
        if not (sym):
            return apology("stock doesn't exist")
        n = int(request.form.get("shares"))
        if (n <= 0):
            return apology("Number has to be positive int")    
        amount = db.execute("SELECT SUM(amount) FROM transactions WHERE user_id = ? AND symbol = ?", session["user_id"], sym["symbol"])[0]["SUM(amount)"]
        amount = int(amount)
        if (amount < n):
            return apology("You don't own that many")    

        # Register transaction
        db.execute("INSERT INTO transactions (symbol, price, amount, user_id) VALUES (?,?,?,?)", 
                   sym["symbol"], 
                   sym["price"],
                   (-n),
                   session["user_id"])
        
        # Add cash
        db.execute("UPDATE users SET cash = (cash + ?) WHERE id = ?", (sym["price"] * n) , session["user_id"])
        
        return redirect("/")

    else:
        return render_template("sell.html")
