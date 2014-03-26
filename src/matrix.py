#
# File: matrix.py
# Modified: Philip Maechling
# Purpose: Define a simply python class for creating matric
# Usage: initialize matrix with matrix(col,row)
# Output: Program will create a string representation
#         by print matrix which calls the to-string method
# Note that indices are (1 to num) not (0 to num-1)
#
class Matrix(object):
    def __init__(self, cols, rows):
        self.cols = cols
        self.rows = rows
        # initialize matrix and fill with zeroes
        self.matrix = []
        for i in range(rows):
            ea_row = []
            for j in range(cols):
                ea_row.append(0)
            self.matrix.append(ea_row)
    def setitem(self, col, row, v):
        self.matrix[col-1][row-1] = v
        
    def getitem(self, col, row):
        return self.matrix[col-1][row-1]
        
    def __repr__(self):
        outStr = ""
        for i in range(self.rows):
            mstr = "%s"%self.matrix[i]
            nstr = mstr.strip('[');
            tstr = nstr.strip(']');
            outStr += '%s\n' % (tstr)
        return outStr
