/*Gabriel Velazquez
 * gvelazq3	
 * asg3
 */

class Matrix {
	// inner class Entry has fields int column and double value 
	private class Entry{
		int column;
		double value;
	
		Entry (int column, double value) {
			this.column = column;
			this.value = value;
		}
		
		// boolean equals method which overrides Object superclass
	      	public boolean equals (Object x){
	      		Class<? extends Object> xClass = x.getClass();
	         	String classN = xClass.getName();
	         	if (!classN.equals("Entry")){
	            	return (this == x);
	        	}
	         
	         	Entry e = (Entry) x;
	        	return (e.column == column && e.value == value);
	      	 }
		
	    // toString prints the Entry
	      	@SuppressWarnings("unused")
			public String toString() {
	      		//if null return space, otherwise return column and value.
	      		if (this==null) {
	      			return " ";
	      		}
	      		else {
	      		String printString = "(" + column + ", " + value + ")";
	      		return printString;
	      		}
	      	    }
	      	}
		
	//matrix fields
	List[] rows;
	int size = 0;
	int NNZ;
		
		
	
	// Constructor 
	// Makes a new n x n zero Matrix. pre: n>=1 
			 		
	Matrix(int n) {
		//declare variables
		size = n;
		NNZ = 0;
		rows = new List[size+1];
		
		for (int i = 1; i <= size; ++i) {
			rows[i] = new List();
		}
	}
	// Access functions 
	// Returns n, the number of rows and columns of this Matrix 
	int getSize() {
		return size;
	}
	
	// Returns the number of non-zero entries in this Matrix 
	int getNNZ() {
		return NNZ;
	}
	
	// overrides Object's equals() method 
	public boolean equals(Object x) {
		if (!(x instanceof Matrix)) {
			return false;
		}
		if (x == this) {
			return true;
		}
	
	 
	Matrix M = (Matrix)x;
	List A;
	List B;
	boolean flag = true;
	
	//checks if same or not same
	if (getSize() == M.size && getNNZ() == M.getNNZ()) {
		int i = 0;
        A = this.rows[i];
        while( (flag) && (A != null) && (i < size)) {
          A = rows[i];
          B = M.rows[i];
          flag = A.equals(B);
          ++i;
        }
        return flag;
      }
      return false;
   }
	
	// Manipulation procedures 
	// sets this Matrix to the zero state 
	void makeZero() {
		for (int i = 1; i <= size; i++) {
			rows[i].clear();
		}
		NNZ = 0;
	}
		
	// returns a new Matrix having the same entries as this Matrix 
	Matrix copy() {
	      Matrix copyMatrix = new Matrix (rows.length);
	      copyMatrix.size = size;
	      Entry A;
	      for (int i = 1; i <= size; i++) {
	         rows[i].moveTo(0);
	         while (!this.rows[i].offEnd()) {
	           A = new Entry(((Entry)(rows[i].getElement())).column, ((Entry)(rows[i].getElement())).value);
	           copyMatrix.rows[i].append(A);
	           copyMatrix.NNZ++;
	           rows[i].moveNext();
	         }
	      }
	      return copyMatrix;
	   } 
	
	
	// changes ith row, jth column of this Matrix to x 
	// pre: 1<=i<=getSize(), 1<=j<=getSize() 
	void changeEntry(int i, int j, double x){
	       if (x == 0) {
	       if (rows[i].length() > 0) {
	           //move to position (0,0)
	           rows[i].moveTo(0);
	           while ((!rows[i].offEnd()) && ((Entry)rows[i].getElement()).column < j) {
	             rows[i].moveNext();
	           }
	           if (!rows[i].offEnd()) {
	             // delete the old entry
	             if (((Entry)(rows[i].getElement())).column == j) {
	               rows[i].delete();
	               NNZ--;
	             }
	           }
	         }
	       }
	       // insert new entry
	       if (x != 0){
	         Entry entry = new Entry(j, x);
	         if (rows[i].length() == 0) {
	           rows[i].append(entry);
	           NNZ++;
	         } else {
		   //move to position (0,0)
	           rows[i].moveTo(0);
	           // find position
	           while (!rows[i].offEnd() && ((Entry)rows[i].getElement()).column < j) {
	             rows[i].moveNext();
	           }
		   int pp;
	           if (rows[i].offEnd()) {
	             rows[i].append(entry);
	             NNZ++;
	           }
	           // check if the same
	           else if (j == ((Entry)rows[i].getElement()).column) {
	             rows[i].delete();
	             rows[i].insertBefore(entry);
		   }
	           else if (j < (pp=((Entry)rows[i].getElement()).column)) {
	             rows[i].insertBefore(entry);
	             NNZ++;
	           }
	           else if (j > (pp=((Entry)rows[i].getElement()).column)) {
	             rows[i].insertAfter(entry);
	             NNZ++;
		   }
	         }
	       }
	     }
	
	// returns a new Matrix that is the scalar product of this Matrix with x 
	Matrix scalarMult(double x){
	      Matrix m = new Matrix(size);

	      for (int i = 1; i <= size; i ++) {
	         int actual = i;
	         if (!rows[actual].isEmpty()) {
	            rows[actual].moveTo(0);

	            while (!rows[actual].offEnd()) {
	               Entry e = (Entry)rows[actual].getElement();
	               int j = e.column;
	               m.changeEntry(i, j, x*(e.value));
	               rows[actual].moveNext();
	            }
	         }
	      }
	      return m;
	   }
	 
	// returns a new Matrix that is the sum of this Matrix with M 
	// pre: getSize()==M.getSize() 
	Matrix add(Matrix M){
	      if (getSize() != M.getSize()) {
	          throw new RuntimeException ("Error:  pre: matrices must be same size");
	       }

	       Matrix sum = new Matrix (size);
	          for (int i = 1; i <= size; i++){
	             //get columns, values, append, and move based on rows
		     //if, else if, and else statements on how to proceed depending on 
		     //empty or not

	             if (!rows[i].isEmpty() && M.rows[i].isEmpty()) {
	               rows[i].moveTo(0);
	               while (!rows[i].offEnd()) {
	                 Entry E = (Entry) rows[i].getElement();
	                 Entry F = new Entry (E.column, E.value);
	                 sum.rows[i].append(F);
	                 ++sum.NNZ;
	                 rows[i].moveNext();
	               }
	             }
	             else if (rows[i].isEmpty() && !M.rows[i].isEmpty()) {
	               M.rows[i].moveTo(0);
	               while (!M.rows[i].offEnd()) {
	                 Entry E = (Entry) M.rows[i].getElement();
	                 Entry F = new Entry (E.column, E.value);
	                 sum.rows[i].append(F);
	                 ++sum.NNZ;
	                 M.rows[i].moveNext();
	               }
	             }
	             else if (!rows[i].isEmpty() && !M.rows[i].isEmpty()) {
	               rows[i].moveTo(0);
	               M.rows[i].moveTo(0);
	               while (!rows[i].offEnd() && !M.rows[i].offEnd()) {
	                 Entry E = (Entry) rows[i].getElement();
	                 Entry F = (Entry) M.rows[i].getElement();
	                 
	                 //if column E is less than column F
	                 //append E
	                 if (E.column < F.column){
	                   sum.rows[i].append(E);
	                   rows[i].moveNext();
	                   } else if (E.column > F.column){
	                      sum.rows[i].append(F);
	                      ++sum.NNZ;
	                      M.rows[i].moveNext();
	                   } else {
	                      Entry G = new Entry (E.column, E.value+F.value);
	                      if (G.value != 0) {
	                         sum.rows[i].append(G);
	                         ++sum.NNZ;
	                      }
	                      //if this is not equal to M
	                      if (this != M)
	                         rows[i].moveNext();
	                      M.rows[i].moveNext();
	                   }
	                }
	                while (!rows[i].offEnd()) {
	                 Entry E = (Entry) rows[i].getElement();
	                 sum.rows[i].append(E);
	                 ++sum.NNZ;
	                 rows[i].moveNext();
	                }
	                while (!M.rows[i].offEnd()) {
	                 Entry E = (Entry) M.rows[i].getElement();
	                 sum.rows[i].append(E);
	                 ++sum.NNZ;
	                 M.rows[i].moveNext();
	                }
	             }
	          }
	       return sum;
	    }
 
	 
	// returns a new Matrix that is the difference of this Matrix with M 
	// pre: getSize()==M.getSize() 
	Matrix sub(Matrix M){
	      if (M.getSize() != size){
	          throw new RuntimeException("Error: pre: matricies must be same size");
	       }

	       Matrix diff = new Matrix (size);
	       for (int i = 1; i <= size; i++){
		  //if, else if, and else statements on how to proceed depending on 
		  //empty or not
	          if (!rows[i].isEmpty() && M.rows[i].isEmpty()){
	             rows[i].moveTo(0);
	             while (!rows[i].offEnd()){
	                Entry A = (Entry)rows[i].getElement();
	                Entry B = new Entry (A.column, A.value);
	                diff.rows[i].append(B);
	                ++diff.NNZ;
	                rows[i].moveNext();
	             }
	          } else if (rows[i].isEmpty() && !M.rows[i].isEmpty()) {
	             M.rows[i].moveTo(i);
	             while (!M.rows[i].offEnd()){
	                Entry A = (Entry)M.rows[i].getElement();
	                Entry B = new Entry (A.column, -(A.value));
	                diff.rows[i].append(B);
	                ++diff.NNZ;
	                M.rows[i].moveNext();
	             }
	          } else if (!rows[i].isEmpty() && !M.rows[i].isEmpty()) {
	             rows[i].moveTo(0);
	             M.rows[i].moveTo(0);
	             while (!rows[i].offEnd() && !M.rows[i].offEnd()){
	                Entry A = (Entry)rows[i].getElement();
	                Entry B = (Entry)M.rows[i].getElement();
	                int cd = A.column - B.column;
	                //sets entry depending on value of cd
	                if (cd < 0){
	                   diff.rows[i].append(A);
	                   ++diff.NNZ;
	                   rows[i].moveNext();
	                } else if (cd >0){
	                   Entry C = new Entry(A.column, -(B.value));
	                   diff.rows[i].append(C);
	                   ++diff.NNZ;
	                   M.rows[i].moveNext();
	                } else{
	                   Entry D = new Entry(A.column, A.value - B.value);
	                   if (D.value != 0){
	                     diff.rows[i].append(D);
	                     ++diff.NNZ;
	                   }
	                   // if not equal to M moveNext is called.
	                   if (this != M)
	                      rows[i].moveNext();
	                   M.rows[i].moveNext();
	                }
	             }
	          }
	          while (!rows[i].offEnd()){
	             Entry E = (Entry)rows[i].getElement();
	             diff.rows[i].append(E);
	             ++diff.NNZ;
	             rows[i].moveNext();
	          }
	          while (!M.rows[i].offEnd()){
	             Entry F = (Entry)M.rows[i].getElement();
	             Entry G = new Entry(F.column, -(F.value));
	             diff.rows[i].append(G);
	             ++diff.NNZ;
	             M.rows[i].moveNext();
	          }
	       }
	       return diff;
	    }

	
	// returns a new Matrix that is the transpose of this Matrix 
	Matrix transpose(){
		
	Matrix M = new Matrix(size);
    for (int i = 1; i <=size; i++) {
       int actual = i;
       if (!rows[actual].isEmpty()) {
          rows[actual].moveTo(0);
          while (!rows[actual].offEnd()) {
             Entry A = (Entry)rows[actual].getElement();
             M.changeEntry(A.column, i, A.value);
             rows[actual].moveNext();
          }
       }
    }
    return M;
 }
 
	 
	// returns a new Matrix that is the product of this Matrix with M 
	// pre: getSize()==M.getSize() 
	Matrix mult(Matrix M){
	      if (M.getSize() != size) {
	          throw new RuntimeException ("Error: size of matrices must be equal");
	       }

	       Matrix product = new Matrix (size);
	       Matrix transp = M.transpose();

	       for (int i = 1; i <= size; i++) {
	          List A = rows[i];
	          if (!A.isEmpty()){
	             for (int j = 1; j <= size; j++) {
	                List B = transp.rows[j];
	                if (!B.isEmpty()){
	                   double val = dot(A, B);
	                   product.changeEntry(i, j, val);
	                }
	             }
	          }
	       }
	       return product;
	    }

	    // Dot Product
	    // computes the vector dot product of two matrix rows represented by Lists P and Q.
	    private static double dot (List P, List Q) {
	       P.moveTo(0);
	       Q.moveTo(0);

	       double dotProduct = 0.0;

	       while (!P.offEnd() && !Q.offEnd()) {
	          Entry A = (Entry)P.getElement();
	          Entry B = (Entry)Q.getElement();
	          int cmp = A.column - B.column;
	          if (cmp > 0) {
	             Q.moveNext();
	          } else if (cmp < 0) {
	             P.moveNext();
	          } else {
	             dotProduct += (A.value*B.value);
	             P.moveNext();
	             Q.moveNext();
	          }
	       }
	       return dotProduct;
	    }
 
	 
	 
	// Other functions 
	// overrides Object's toString() method 
	public String toString(){
	       //if the size is 0 return blank
		   if (size == 0)
	          return "";

	    String result = new String("");

	     for (int i = 1; i <= getSize(); i++){
	       if (rows[i].length() > 0){
	         result = result + String.valueOf(i) + ":";
	         rows[i].moveTo(0);
	         while (!rows[i].offEnd()){
	           result = result + ((Entry)(rows[i].getElement()));
	           rows[i].moveNext();
	         }
	         result = result + "\n";
	       }
	     }
	     return result;
	    }
	 
}


