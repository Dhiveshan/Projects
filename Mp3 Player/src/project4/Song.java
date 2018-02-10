package project4;


public class Song {
	private String name;
	private String author;
	private String fileName;
	
	public Song(String name, String author, String dir)
	{
		this.name = name;
		this.author = author;
		this.fileName = dir;
	}

	public String getName() {
		return name;
	}

	public String getAuthor() {
		return author;
	}

	public String getFileName() {
		return fileName;
	}

	public String toString()
	{
		return name + System.lineSeparator() + author + System.lineSeparator() + fileName;
	}
}
