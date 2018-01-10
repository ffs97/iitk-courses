consonants = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ"

consonant_rep = input("Input number of times a consonant will be repeated: ")
vowel_rep = input("Input number of times the vowel between each repetition of a consonant will be repeated: ")

vowel = raw_input("Input the vowel to be repeated: ")
if not vowel in vowels:
	print "Not a vowel"
else:

	input_str = raw_input("Please input a String:\n")
	generated_str = input_str

	for letter in consonants:
		generated_str = generated_str.replace(letter, ((letter+vowel*vowel_rep)*consonant_rep)[:-vowel_rep])

	print generated_str