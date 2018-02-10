package project4;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Iterator;

import org.jaudiotagger.audio.AudioFile;
import org.jaudiotagger.audio.AudioFileIO;
import org.jaudiotagger.audio.exceptions.CannotReadException;
import org.jaudiotagger.audio.exceptions.InvalidAudioFrameException;
import org.jaudiotagger.audio.exceptions.ReadOnlyFileException;
import org.jaudiotagger.tag.FieldKey;
import org.jaudiotagger.tag.Tag;
import org.jaudiotagger.tag.TagException;

import javazoom.jl.decoder.JavaLayerException;
import javazoom.jl.player.Player;

public class SongDatabase {

	private SongList sList;
	private SongList newList;
	private int numSongs =0;
	public SongDatabase()
	
	{
		sList = new SongList();
		newList = new SongList();
	}
	public void LoadSongs(File[] dir)
	{
	try{
			for(int i=0;i<dir.length;i++)
			{
				if(dir[i].toString().endsWith(".mp3"))
				{
					String[] splitName = dir[i].toString().split("/");
					int len = splitName.length;
					AudioFile f = AudioFileIO.read(new File(dir[i].toString()));
					Tag tag = f.getTag();
					String artist =tag.getFirst(FieldKey.ARTIST);
					Song song = new Song(dir[i].getName(),artist,dir[i].toString());
					sList.insertByTitle(song);
					numSongs++;
				}
			}
			sList.printNodes();
	}
	catch(IOException e)
	{
		System.out.println("The input was not correct");
	}
	catch(ReadOnlyFileException a)
	{
		System.out.println("The input was not correct");
	}
	catch(InvalidAudioFrameException x)
	{
		System.out.println("The input was not correct");
	}
	catch(CannotReadException d)
	{
		System.out.println("The input was not correct");
	}
	catch(TagException g)
	{
		System.out.println("The input was not correct");
	}
	
	}
	public void searchSong(String songName)
	{
		if(sList.find(songName))
		{
			sList.addSongstoPlayer(newList, songName);
			newList.printNodes();
		}
		else
		{
			System.out.println("There is no song that contains these keywords");
		}
		
	}
	public void stopSong(int num)
	{
		MyThread t = new MyThread(num);

	t.stop();
		

	}
	
	public void playSong(int num)
	{
		MyThread t = new MyThread(num);

	t.start();
		

	}
	public Iterator<SongNode> newIt()
	{
		return newList.iterator();
	}
	public void addToPlayer()
	{
		
		sList.addToPlayer(newList);
	}
	public Iterator<SongNode> it()
	{
		return sList.iterator();
	}
	public int getNumSongs()
	{
		return numSongs;
	}
	public class MyThread extends Thread
	{
		private int n;
		public MyThread(int n)
		{
			this.n = n;
		}
		public void run()
		{
			
			Player pl;
			FileInputStream file;
			Song playing = newList.findIndexElem(n);
			try
			{

				file = new FileInputStream(playing.getFileName());
				pl = new Player(file);
				System.out.println("Song playing" + ": " + playing.getName());
				pl.play();
				if(n<0)
				{
					file.close();
				}
				
			}
			
			catch(FileNotFoundException e)
			{
				e.getMessage();
			}
			catch(JavaLayerException e)
			{
				e.getMessage();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
}
