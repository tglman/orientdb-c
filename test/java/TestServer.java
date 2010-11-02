import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class TestServer {

	public static void main(String[] args) {
		try {
			ServerSocket s = new ServerSocket(3333, 0,
					InetAddress.getByName("127.0.0.1"));
			Socket socket = s.accept();

			DataOutputStream out = new DataOutputStream(socket.getOutputStream());
			DataInputStream in = new DataInputStream(socket.getInputStream());
			
			
			int val = in.readInt();
			out.writeInt(val);
			
			byte val_c = in.readByte();
			out.writeByte(val_c);

			
			short val_s = in.readShort();
			out.writeShort(val_s);

			
			long val_64 = in.readLong();
			out.writeLong(val_64);

			int number = in.readInt();
			byte [] content = new byte[number];
			in.read(content);
			out.writeInt(number);
			out.write(content);
			
			int char_number = in.readInt();
			byte [] char_content = new byte[char_number];
			in.read(char_content);
			String str = new String(char_content);
			out.writeInt(str.length());
			out.write(str.getBytes());
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
