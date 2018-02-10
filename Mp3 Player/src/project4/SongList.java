package project4;

import java.util.Iterator;

public class SongList {
	private SongNode head, tail;
	//private SongNode current;
	
	public SongList()
	{
		head = null;
		tail = null;
	}
	
	public void insertAtFront(Song elem)
	{
		SongNode newNode = new SongNode(elem);
		if(head != null)
		{
			newNode.setNext(head);
		}
		else
		{
			tail = newNode;
		}
		head = newNode;
	}
	public void insertByTitle(Song elem)
	{
		
		SongNode newNode = new SongNode(elem);
		if(head == null)
		{
			newNode.setNext(head);
			head = newNode;
			tail = newNode;
		}
		else if(head.next()==null)
		{
			SongNode other = head;
			int res1;
			res1 = head.elem().getName().compareTo(elem.getName());
			if(res1 <0)
			{
				this.append(elem);
			}
			else if(res1 >0)
			{
				
				insertAtFront(elem);
				
				
			}
			else if(res1 ==0)
			{
				tail.setNext(newNode);
				tail = newNode;
			}
		}
		else
		{
			int result1;
			int result2;
			SongNode current = head;
			SongNode next = head.next();
			while(current != null)
			{
				result1 = current.elem().getName().compareTo(elem.getName());
				System.out.println(result1);
				result2 = next.elem().getName().compareTo(elem.getName());
				System.out.println(result2);
				if(result1 >0 && result2 <0)
				{
					current.setNext(newNode);
					newNode.setNext(current);
					
				}
				else if(result1<0 && result2 <0)
				{
					if(current.next() ==null)
					{
						append(elem);
					}
					else
					{
						
					}
					
				}
				else if(result1 <0 && result2 >0  )
				{
					newNode.setNext(current.next());
					current.setNext(newNode);
					
					
				}
				else if(result1==0 && result2 >0)
				{
					
				}
				
				current =current.next();
				if(next.next() != null)
				{
					next = next.next();
				}
			}
		}
	}
		
	public void append(Song elem)
	{
		SongNode newNode = new SongNode(elem);
		if(tail != null)
		{
			tail.setNext(newNode);
			tail = newNode;
		}
		else
		{
			head = tail = newNode;
		}
	}
	public void printNodes()
	{
		SongNode current = head;
		while(current != null)
		{
			System.out.println(current.elem().getName());
			current = current.next();
		}
	}
	public boolean find(String elem)
	{
		SongNode current = head;
		while(current != null)
		{
			if(current.elem().getName().contains(elem))
			{
				return true;
			}
			current = current.next();
		}
		return false;
	}
	public void addSongstoPlayer(SongList sc, String elem)
	{
		SongNode current = head;
		while(current != null)
		{
			if(current.elem().getName().contains(elem))
			{
				sc.insertByTitle(current.elem());
			}
			current = current.next();
		}
	}
	public Song remove(SongNode previousNode)
	{
		if((previousNode == null) || (previousNode.next() == null))
		{
			System.out.println("Nothing to remove");
			return null;
		}
	
	Song elem = previousNode.next().elem();
	if(previousNode.next() == tail)
	{
		tail = previousNode;
		tail.setNext(null);
	}
	else
	{
		previousNode.setNext(previousNode.next().next());
	}
	return elem;
	}
	public Iterator<SongNode> iterator()
	{
		return new MyIterator();
	}
		class MyIterator implements Iterator<SongNode>
		{
			SongNode current;
			MyIterator()
			{
				current = head;
			}
			@Override
			public boolean hasNext()
			{
				return (current != null);
			}
			@Override
			public SongNode next()
			{
				if(!hasNext())
					return null;
				SongNode tmp = current;
				current = current.next();
				return tmp;
			}
		}
		public void addToPlayer(SongList sc)
		{
			SongNode current = head;
			while(current != null)
			{
				sc.insertAtFront(current.elem());
			}
			current = current.next();
		}
		public Song findIndexElem(int n)
		{
			SongNode current = head;
			int count =0;
			if(current.elem() != null)
			{
			while(count <n && current.elem() != null)
			{
				current = current.next();
				count++;
			}
			return current.elem();
			}
			else
			{
				return null;
			}
		}
}
