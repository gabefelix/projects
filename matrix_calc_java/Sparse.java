/*Gabriel Velazquez
 * gvelazq3	
 * asg3
 */

import java.io.*;
import java.util.*;
import java.util.Scanner;

class Sparse {
	
	public static void main (String[]args) throws IOException{

		//if there isn't two files, print the usage, then exit.
		if(args.length !=2) {
			System.out.println("Usage: FiloIO infile outfile");
			System.exit(1);
		}
		Scanner in = null;
		PrintWriter out = null;
		String line = null;
      		String[] token = null;
	
		
		
		//scan the file
		in = new Scanner(new File(args[0]));
		out = new PrintWriter(new FileWriter(args[1]));
		
		//declare the variable
		int n = 0, aNNZ = 0, bNNZ = 0;
		
		try{
		n = in.nextInt();
		aNNZ = in.nextInt();
		bNNZ = in.nextInt();
		}
		catch (InputMismatchException exception) 
		{ 
		    System.out.println("InputMismatchException: Integers only/Not correctly formatted. Please fix file. "); 
		    System.exit(1);
		} 
		catch (NoSuchElementException exception) 
		{ 
		    System.out.println("NoSuchElementException: Integers only/Not correctly formatted. Please fix file. "); 
		    System.exit(1);
		} 

		Matrix A = new Matrix(n);
		Matrix B = new Matrix(n);
		
		for (int i = 0; i < aNNZ; ++i) {
			int row = in.nextInt(); //try catch
			int column = in.nextInt();
			double value = in.nextDouble();
			A.changeEntry(row, column, value);
		}
		
		for (int i = 0; i <bNNZ; ++i) {
			int row = in.nextInt(); //try catch
			int column = in.nextInt();
			double value = in.nextDouble();
			B.changeEntry(row, column, value);
		}
		
		//print out the functions
		
 		out.println("A has " + aNNZ + " non-zero entries.");
		out.print(A);
		out.println("B has " + bNNZ + " non-zero entries.");
		out.print(B);
		
		//scalar multiplication
		Matrix C = A.scalarMult(1.5);
		out.println("(1.5)*A = ");
	   	out.print(C);
	    
		//add
	    	Matrix D = A.add(B);
	    	out.println("A+B = ");
	    	out.println(D);
	    
		//add
	    	Matrix Z = A.add(A);
	    	out.println("A+A = ");
	    	out.println(Z);
	    
		//subtract
	    	Matrix Y = A.sub(A);
	    	out.println("A-A = ");
	    	out.println(Y);
	    
		//transpose
	    	Matrix F = A.transpose();
	    	out.println("Transpose(A) = ");
	    	out.println(F);
	
		//multiply    
	    	Matrix G = A.mult(B);
	    	out.println("A*B = ");
	    	out.println(G);
	    
		//multiply
	    	Matrix H = B.mult(B);
	    	out.println("B*B = ");
	    	out.println(H);
	    
	    	in.close();
	    	out.close();
	    
	
	
	} 
	

}
