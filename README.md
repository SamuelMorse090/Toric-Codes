# Toric-Codes

Weightcalc - calculates the weight distribution of each position of minimum weight codewords, punctures the code in that position and repeats
Multithreading - multithreaded minimum distance approximator
load_and_continue_generating - Generates affine equivalent lattice triangles in successively larger grids, can also pick up where it left off (not very efficient
because python but not terrible)

LinearCodes.db - SQLite database of codes with ID, [n,k,d] and generating matrix

Biglist.txt - lookup for code IDs in the database and triangle verticies+interior points
