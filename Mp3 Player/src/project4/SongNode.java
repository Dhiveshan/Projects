package project4;

public class SongNode {
	private Song elem;
	private SongNode next;
	public SongNode(Song elem, SongNode next)
	{
		this.elem = elem;
		this.next = next;
	}
	public SongNode(Song elem)
	{
		this.elem = elem;
		next = null;
	}
	public Song elem()
	{
		return elem;
	}
	public void setElem(Song elem)
	{
		this.elem = elem;
	}
	public SongNode next()
	{
		return next;
	}
	public void setNext(SongNode other)
	{
		next = other;
	}
}
