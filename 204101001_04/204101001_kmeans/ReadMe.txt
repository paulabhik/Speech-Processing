Universe contains all the vectors
After the final Execution, all the final codebook vectors are stored in Codebook.txt

Program can be run by simply building (or re-building) it and then running it

Logic

1. Initialiation -- We randomly choose 8 vectors from universe as the initial set of
   code words in th codebook
2. In each iteration of K means, we Apply the nearest neighbour rule and find the 
    code word vector which is nearest from the universe and assign that vector to 
    the corresponding cell
3. We compute the centroid of each cell, and store it back in Codebook
4. Repeat step 2 and 3 unless the percent change in average distortion is less than
   threshold