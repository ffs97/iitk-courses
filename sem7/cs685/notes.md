# Data Reduction

## Curse of Dimensionality

- Data becomes too sparse
- Data is mostly at the boundaries
- Maximum to minimum distances always become equal
- Density becomes irrelevant and therefore clustering is not possible

## Singular Value Decomposition (SVD)

This simply means factorizing a matrix
A = U \S V*

Columns of U are the eigenvectors of A.A*, and columns of V are eigenvectors of
A*.A

Both U and V are orthogonal, and \S is a diagonal matrix, where \s_i,i are the
singular values (positive square roots of A.A* or A*.A)

#### Transformation using SVD

V is called *SVD Transform Matrix*. T = A.V is a rotation of A, and is of the
same dimensionality. This means that the lengths of vectors are preserved =>
||a_i||_2 = ||t_i||_2

If A = A*, then SVD is known as *Spectral Decomposition*, and eigenvalues and
eigenvectors of A would be the same as those of A^2 (eigenvalues would be square
roots)

#### Dimensionality Reduction

Choose singular values such that *energy* preserved is maximum. Energy is
defined as the sum of squares of the singular values.

## Principal Component Analysis

Compute correlation matrix of A, C and find eigenvalues and eigenvectors of C.
Project A to top (highest eigenvalue) eigenvectors.

If A is mean centered, c = A*A and therefore this is the same as SVD in that
case

PCA is also known as Karhunen-Loeve Transform (KLT). This only works for L_2 as
other distances are not invariant to rotation.

# Numerosity Reduction

## Aggregation

Aggregate subset of features to one. Benefits
- Reduces noise by absorbing individual errors
- Lesser variability

## Sampling

Simple Random Sampling Without Replacement (SRSWOR) creates *population*.
Simple Random Sampling With Replacement (SRSWR)

**Stratified Sampling**: For different groups, pick equal or representative
number of objects

### Sampling Size

**Progressing** or **Adaptive Sampling**: Start with a small sample size, and
keep on increasing it.

## Histograms

**Equi-width**: bins are equally spaced apart
**Equi-height** or **Equi-Depth**: bins have the same height
**MaxDiff**: Bin divisions are decided at points where diff is maximum (from the
adjacent point)

## Data Summarization

5-number summary - minimum, first Q, median, third Q, maximum

**Distributive**: Can be computed by partitioning the dataset
**Holistic**: Cannot be computed by partitioning the dataset

## Features Selection

**Filter**: Predetermine features to cull out
**Wrapper**: Feature selection is targeted to the algorithm
**Embedded**: Algorithm has built in feature selection (LASSO Regression)

Add feature that decreases the error most, or remove that increases the error
least.

Mutual Information I(X, Y) = KL(p(x, y)|p(x)p(y))

### Relief Algorithm

Feature score is computed for each feature

# Discretization

**Supervised**: If discretization uses information about groups

Discretization leads to conceptualization.

**Concept Hierarchy**: Captures basic concepts at the top and finer details
towards the leaves.

### Entropy Based

**Entropy**: Captures amount of randomness by encoding the expected number of
bits required to describe a distribution.
entropy(D) = - \sum_i=1^n p_i log(p_i)

For split D_i, entropy_i is calculated, and expected information requirement is
calculated
info_req = \sum_i=1^n entropy_i . |D_i| / |D|
info_gain = entropy(D) - info_req

### Chi-Squared

