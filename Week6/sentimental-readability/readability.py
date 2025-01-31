import re

text = input("Text: ")

words = len(text.split(" "))
sent = len(re.split(r"(?:\.|\!|\?)", text)) -1
print("words: ", words)
print("Sentences: ", sent)
chars = 0
for c in text:
    if c.isalpha():
        chars += 1


L = chars / words * 100
S = sent / words * 100

lvl = 0.0588 * L - 0.296 * S - 15.8
print(f"{lvl} = 0.0588 * {L} - 0.296 * {S} - 15.8")

if lvl >= 16:
    print("Grade 16+")
elif lvl < 1:
    print("Before Grade 1")
else:
    print(f"Grade {round(lvl)}")