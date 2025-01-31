import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Too little command line arguments")

    # TODO: Read database file into a variable
    with open(sys.argv[1], "r") as file:
        content = file.read()
    
    lines = content.split("\n")
    
    keys = lines[0].split(",")
    keys[0:1] = []
    data = {}
    for line in lines[1:]:
        if line == "":
            break
        line = line.split(",")
        # print(f"key: {keys}\nLine: {line}\n")
        data[line[0]] = {keys[i]: int(line[i+1]) for i in range(len(keys))}

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as sequence:
        seq = sequence.read()

    # TODO: Find longest match of each STR in DNA sequence
    sDic = {}
    for key in keys:
        sDic[key] = longest_match(seq, key)
    
    # TODO: Check database for matching profiles
    for name in data:
        # print(data[name], "=", sDic)
        if data[name] == sDic:
            print(name)
            break
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
