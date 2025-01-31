while True:
    try:
        n = int(input("Height: "))
    except ValueError:
        print("Give a positive int > 8")
    else:
        if 0 < n < 9:
            break


for i in range(1, n+1):
    print(" " * (n-i) + "#" * i + "  " + "#"*i)