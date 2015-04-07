
/*Gabriel Velazquez
 * gvelazq3	
 * asg3
 */

class List {

	private class Node {
		// Fields
		Object data;
		Node next;
		Node prev;

		// Constructor
		Node(Object data) {
			this.data = data;
			next = null;
		}

		// toString: Overides Object's toString method.
		public String toString() {
			return String.valueOf(data);
		}
	}

	// Fields
	private Node front, back, curr;
	// length of list
	private int length, index;

	// Constructor, initialize values

	// set front, back, and current
	// return the length of the list
	
	List() {
		front = back = curr = null;
		length = 0;
		index = -1;
	} 


	// getLength(): returns length of this list
	int getLength() {
		return length;
	}

	// isEmpty(): returns true if this is an empty list, false otherwise
	boolean isEmpty() {
		return length == 0;
	}

	boolean offEnd() {
		if (this.curr == null)
			return true;
		else
			return false;
	}


	// toString(): overides Object's toString() method.
	public String toString() {
		String str = "";
		for (Node N = front; N != null; N = N.next) {
			str += N.toString() + " ";
		}
		return str;
	}

	// equals(): returns true if this list is identical to L, false
	// otherwise.
	boolean equals(List L) {
		boolean flag = true;
		Node N = this.front;
		Node M = L.front;

		if (this.length == L.length) {
			while (flag && N != null) {
				flag = (N.data == M.data);
				N = N.next;
				M = M.next;
			}
			return flag;
		} else {
			return false;
		}
	}



	// Access functions
	// Returns number of elements in this list.
	int length() {
		return length;
	} 

	// Returns the index of the cursor element in this list, or
	// returns -1 if the cursor element is undefined.
	
	int getIndex() {
	 	return index;
	}

	// Returns front element in this list. Pre: length()>0

	Object front() {
		if (this.isEmpty() || length() < 0) {

			throw new RuntimeException("List Error: front() called on empty List.");
					
		}
		return front.data;
	}

	// Returns back element in this List. Pre: length()>0

	Object back() {
		if (this.isEmpty() || length() < 0) {
			throw new RuntimeException("List Error: back() called on empty List. ");
		}
		return back.data;
	}

	
	// Returns cursor element in this list.
	// Pre: length()>0, getIndex()>=0
	
	Object getElement() {
		// if list is not empty or index is not -1
      		if (length() > 0 && getIndex() >= 0) {
       			return curr.data;
    		} else {
       			throw new RuntimeException("List Error: Element() called on empty List.");
     		}
   	}
	// Re-sets this List to the empty state.

	void clear() {
    	 	this.back = this.curr = this.front = null;
    	 	length = 0;
    		index = -1;
   	}

	// If 0<=i<=length()-1, moves the cursor to the element
	// at index i, otherwise the cursor becomes undefined.

	
	void moveTo(int new_index) {
		if (new_index >= 0 && new_index <= length()-1) {
      		  index = 0;
       	          curr = front;
       	          while (new_index != getIndex()) {
         	    if (new_index < getIndex()) {
                   	 movePrev();
         	    }
         	    if (new_index > getIndex()) {
           		if (getIndex() < 0) {
            		  curr = front;
             		  index = 0;
             		  if (new_index > getIndex()) {
               			moveNext();
            		  }
           		} else {
             		  moveNext();
           	       }  
                     }
                    }
                  } else {
                    curr = null;
                    index = -1;
                }
         }

	
	//move back
	void movePrev() {

		if (getIndex() > 0 && getIndex() <= length() - 1) {
      		   System.out.println(length() + " " + curr + " " + getIndex());
       		   curr = curr.prev;
       		   index--;
     		} else if (getIndex() == 0) {
      	 	  curr = null;
                  index = -1;
     		} else if (getIndex() == -1) {
                  curr = null;
                  index = -1;
                }
         }

	void moveNext() {

		

		if (getIndex() < length() - 1) {
			curr = curr.next;
			index++;

		} else if (getIndex() == length() -1) {
       			curr = null;
       			index = -1;
    		} else if (getIndex() == -1) {
			curr = null;
       			index = -1;
    		}
   	}

	// Inserts new element before front element in this List.
	
	void prepend(Object data) {
		Node N = new Node(data);
		if (this.isEmpty()) {
			front = back = N;
			curr  = N;
			index = 0;
			length++;		
		} else {
			N.next = front;
			front.prev = N;
			front = N;
			length++;
      			index++;

		}
	} 

	void append(Object data) {
		// create a new node to append
		Node N = new Node(data);
		// if list is empty, set front, back to new node
		if (this.isEmpty()) {
			front = back = N;
			curr  = N;
                        index = 0;
                        length++;
		} else {
			back.next = N;
			N.prev = back;
			back = N;
			length++;
		}
		

	}

	// Inserts new element after back element in this List.

	void insertBefore(Object data) {
	     if (length() > 0 && getIndex() >= 0) {
		Node N = new Node(data);
		N.next = curr;
		N.prev = curr.prev;
		if (curr == front)
		    front = N;
		length++;
		if (getIndex() > 0) {
		    curr.prev.next = N;
		}
		curr.prev = N;
	     }
	}

	// Inserts new element before cursor element in this

	void insertAfter(Object data) {
		 if (length() > 0 && getIndex() >= 0) {
       		 Node N = new Node(data);
      		 N.prev = curr.prev;
       		 curr.prev.next = N;
      		 N.next = curr.next;
       		 curr.next.prev = N;
                 length++;
		 if (curr == back)
		     back = N;
		}
	}

       // Deletes the front element of the list

       void deleteFront() {
    	 if (length() <= 0) {
      	 throw new RuntimeException ("Error: length of list <=0");
     	} else if (length() == 1) {
       	  back = front = curr = null;
      	  length = 0;
     	} else if (length() == 2) {
       	  front = front.next;
          front.prev.next = null;
          front.prev = null;
          length--;
        } else if (length() > 2) {
          Node N = front.next;
          front.next.prev = null;
          front.next = null;
          front = N;
          length--;
        }
       }

      // Deletes the back element in this List. Pre: length()>0

      void deleteBack() {
     	  if (length() <= 0) {
      	    throw new RuntimeException ("Error: length of list <=0");
    	}
    	  else if (length() == 1) {
            back = front = curr = null;
            length = 0;
        }
          else if (length() == 2) {
            back.prev = null;
            back = front;
            back.next = null;
            length--;
        }
          else if (length() > 2) {
            Node N = back.prev;
            back.prev.next = null;
            back.prev = null;
            back = N;
            length--;
        }
      }

	// Deletes cursor element in this List. Cursor is undefined after this

      void delete() {
     	   if (getIndex() == length() - 1) {
              	back = curr.prev;
       		curr.prev.next = null;
       		curr.next = null;
       		curr.prev = null;
      	 	curr = null;
       		index = -1;
       		length--;
     	} else if (length() > 0 && getIndex() > 0) {
      		curr.prev.next = curr.next;
       		curr.next.prev = curr.prev;
       		curr.prev = null;
       		curr.next = null;
       		curr = null;
       		index = -1;
       		length--;
     	} else if (length() > 1 && getIndex() == 0) {
       		front = curr.next;
       		curr.next.prev = null;
      		curr.next = null;
       		curr = null;
       		index = -1;
       		length--;
     	} else if (length() == 1 && getIndex() == 0) {
       		front = back = curr = null;
       		index = -1;
       		length--;
        }
     
     }

	

}

