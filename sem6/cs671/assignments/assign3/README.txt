_____________________________ CS671: ASSIGNMENT 2 _____________________________

								 Gurpreet Singh
									 150259


______________________________ Problem Statement _______________________________

We need to build a dependency parser for the EWT treebank for English


___________________________________ Approach ___________________________________

Used the conllu library to generate a parse structure for the  data,  then  used
the  transition  algorithm  discussed  in  class  to  parse  the  complete  data
simulating a stack and a buffer.

For each word of a sentence, feature representations are computed based  on  the
POS tag of the word and its dependencies from the stack as well  as  the  buffer
and their corresponding glove  embeddings.   The  output  vector  is  a  one-hot
representation of the  output,  with  the  classes  being  the  left-arc  +  its
dependency relation, right-arc + its dependency relation or a  shift  operation.

The final algorithm is a  classification  algorithm  using  a  black-box  neural
network which  is  trained  using  the  Adam  optimizer  with  default  settings
(scikit-learn)

___________________________________ Accuracy ___________________________________

The accuracy of the learned model on the test data (given in the official
dataset) came out to be equal to  __ 89.205 % __
