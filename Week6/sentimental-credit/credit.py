import sys

n = input("Number: ")

out = 0
for i in n[-2::-2]:
    i = int(i)
    out += (i*2)%10 + (i*2)//10
for i in n[-1::-2]:
    i = int(i)
    out += i

if out % 10 != 0:
    print("INVALID")
    sys.exit(0)


if len(n) == 15 and n[0:2] in ["34", "37"]:
    print("AMEX")
elif len(n) == 16 and 50 < int(n[0:2]) < 56:
    print("MASTERCARD")
elif (len(n) == 13 or len(n) == 16) and n[0] == "4":
    print("VISA")
else:
    print("INVALID")