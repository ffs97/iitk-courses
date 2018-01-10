consonants = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ"

input_str = raw_input("Please input a String:\n")
generated_str = input_str

for letter in consonants:
	generated_str = generated_str.replace(letter, (letter+'o'+letter))

print generated_str