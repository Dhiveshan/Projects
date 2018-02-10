package project4;

import java.io.File;

public class UserInterface {
	private File[] dir;
	public UserInterface(File[] dir)
	{
		this.dir = dir;
	}
	SongDatabase sD = new SongDatabase(dir);

}
