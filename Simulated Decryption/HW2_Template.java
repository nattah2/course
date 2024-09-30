import javax.crypto.Cipher;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Scanner;

// BEGIN SOLUTION
// Please import only standard libraries and make sure that your code compiles and runs without unhandled exceptions
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.util.Arrays;
import java.io.*;
// END SOLUTION
 
public class HW2_Template {
  static void P1() throws Exception {
    byte[] cipherBMP = Files.readAllBytes(Paths.get("cipher1.bmp"));
    
    // BEGIN SOLUTION
    byte[] iv = new byte[] { 0, 0, 0, 0,             // IV is 16 0 bytes.
                             0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0};
    byte[] myKey = new byte[16];                     // myKey
    for (byte i = 0; i < myKey.length; i++) {
      myKey[i] = (byte) (i + 1);
    }
    Cipher cipher = Cipher.getInstance("AES/CBC/ISO10126Padding");
    cipher.init(Cipher.DECRYPT_MODE, new SecretKeySpec(myKey, "AES"), new IvParameterSpec(iv));
    byte[] plainBMP = cipher.doFinal(cipherBMP);
    // END SOLUTION
    Files.write(Paths.get("plain1.bmp"), plainBMP);
  }

  static void P2() throws Exception {
    byte[] cipher = Files.readAllBytes(Paths.get("cipher2.bin"));
    // BEGIN SOLUTION
    byte[] block1 = Arrays.copyOfRange(cipher, 0, 16);
    byte[] block2 = Arrays.copyOfRange(cipher, 16, 32);
    byte[] block3 = Arrays.copyOfRange(cipher, 32, 48);
    byte[] modifiedCipher = new byte[48];
    for (int i = 0; i < 48; i++) {
      if (i < 16) {
        modifiedCipher[i] = block3[i % 16];
      }
      else if (i < 32) {
        modifiedCipher[i] = block2[i % 16];
      }
      else {
        modifiedCipher[i] = block1[i % 16];
      }
    }
    Cipher myCipher = Cipher.getInstance("AES/CBC/NoPadding");
    byte[] iv = new byte[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    byte[] myKey = new byte[16];
    for (byte i = 0; i < myKey.length; i++) {
      myKey[i] = (byte) (i + 1);
    }
    myCipher.init(Cipher.DECRYPT_MODE, new SecretKeySpec(myKey, "AES"), new IvParameterSpec(iv));
    byte[] plain = myCipher.doFinal(modifiedCipher);
    System.out.println(plain);
    // END SOLUTION
    
    Files.write(Paths.get("plain2.txt"), plain);
  }

  static void P3() throws Exception {
    byte[] cipherBMP = Files.readAllBytes(Paths.get("cipher3.bmp"));
    byte[] otherBMP = Files.readAllBytes(Paths.get("plain1.bmp"));
    
    // BEGIN SOLUTION
    byte[] modifiedBMP = cipherBMP;
    for (int i = 0; i < 2000; i++) {
      modifiedBMP[i] = otherBMP[i];
    }
    // END SOLUTION
    
    Files.write(Paths.get("cipher3_modified.bmp"), modifiedBMP);
  }

  static void P4() throws Exception {
    byte[] plainA = Files.readAllBytes(Paths.get("plain4A.txt"));
    byte[] cipherA = Files.readAllBytes(Paths.get("cipher4A.bin"));
    byte[] cipherB = Files.readAllBytes(Paths.get("cipher4B.bin"));
    byte[] myKey = cipherA;
    
    // BEGIN SOLUTION
    //
    byte[] plainB = cipherB;
    for (int i = 0; i < cipherA.length; i++) {
      myKey[i] = (byte) (plainA[i] ^ cipherA[i]);
    }
    for (byte i = 0; i < cipherB.length; i++) {
      plainB[i] = (byte) (myKey[i] ^ cipherB[i]);
    }

    // END SOLUTION
    Files.write(Paths.get("plain4B.txt"), plainB);
  }

  /* First five bytes should be the same as the others */
  static void P5() throws Exception {
    byte[] cipherBMP = Files.readAllBytes(Paths.get("cipher5.bmp"));
    byte[] bmpA = Files.readAllBytes(Paths.get("plain1.bmp"));
    
    // BEGIN SOLUTION
    System.out.println("Searching for the right combination...");
    byte[] key = new byte[16];
    byte[] iv = new byte[16];
      for (int i = 0; i <= 99; i++) {
        key[0] = (byte) i;
        for (int j = 0; j <= 12; j++) {
          key[1] = (byte) j;
          for (int k = 0; k <= 31; k++) {
            key[2] = (byte) k;
            try {
                Cipher cipher = Cipher.getInstance("AES/CBC/ISO10126Padding");
                cipher.init(Cipher.DECRYPT_MODE, new SecretKeySpec(key, "AES"), new IvParameterSpec(iv));
                byte[] plainBMP = cipher.doFinal(cipherBMP);
                boolean check = Arrays.equals(Arrays.copyOfRange(plainBMP, 0, 6), Arrays.copyOfRange(bmpA, 0, 6));
                if (check) {
                  Files.write(Paths.get("plain5.bmp"), plainBMP);
                  System.out.println("Found it!");
                  return;
                }
              }
          catch (BadPaddingException | IllegalBlockSizeException e) {
            // e.printStackTrace();
            continue;
          }
        }
      }
    }
    System.out.println("Couldn't find anything!");
  }

  public static void main(String [] args) {
    try {  
      P1();
      P2();
      P3();
      P4();
      P5();
    } catch (Exception e) {
      e.printStackTrace();
    } 
  }
}
