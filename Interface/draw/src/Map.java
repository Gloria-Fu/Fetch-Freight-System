import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class Map extends Thread
{
    public static int start = 0, gap = 10;
    public static int gridSize = 9;
    public static int width = 147, height = 89;

    private JFrame jFrame = new JFrame();

    @Override
    public void run() {
        jFrame.getContentPane().setBackground(Color.white);
        jFrame.setSize(1900, 1020);

        JPanel jp1 = new JPanel(){
            public void paint(Graphics g){
                super.paint(g);

                try {
                    drawMap(g);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }

                drawConveyor(g);

                try {
                    drawRobots(g);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }

            }
        };
        jp1.setBackground(null);
        jp1.setOpaque(false);
        jFrame.add(jp1);
        jFrame.setVisible(true);
        while (true) {
            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            jp1 = new JPanel() {
                public void paint(Graphics g) {
                    super.paint(g);
                }
            };
            jp1.setBackground(null);
            jp1.setOpaque(false);
            jFrame.add(jp1);
            jFrame.setVisible(true);
            jFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        }
    }



    public static int drawMap(Graphics g) throws FileNotFoundException {

        String path = "E:\\project\\3X3V6\\video\\V0\\map.txt";

        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();
        while (in.hasNextLine()) {
            int num = in.nextInt();

            if(num == -1){
                break;
            }

            int[] arrs = {num};

            data.add(arrs);
        }


        int[] mapNode = new int[5];
        int index = 0;
        for (int i = 0; i < data.size(); i++) {
            int[] arr = (int[]) data.get(i);

            mapNode[index] = arr[0];
            index++;
            if(index == 5) {
                if(mapNode[3] == 0){
                    g.setColor(Color.BLACK);

                }else if(mapNode[3] == 1){
                    g.setColor(Color.white);
                }else if(mapNode[3] == 2){
                    g.setColor(Color.ORANGE);
                }else if(mapNode[3] == 3){
                    g.setColor(Color.red);
                }
                g.fillRect(mapNode[1]*gap, (mapNode[2]+2)*gap, gridSize, gridSize);
                index = 0;
            }

        }

        return 0;
    }

    public static int drawConveyor(Graphics g) {

        g.setColor(Color.LIGHT_GRAY);

        g.fillRect((width - 6)*gap,(start + 4)*gap,5*gap,2*gap);
        g.fillRect((width - 6)*gap, (start + 6)*gap,2*gap, (height - 8)*gap);
        g.fillRect((width - 6)*gap,(height - 2)*gap,5*gap,2*gap);

        g.fillRect(1*gap,(start + 4)*gap,5*gap,2*gap);
        g.fillRect(4*gap, (start + 6)*gap,2*gap, (height - 8)*gap);
        g.fillRect(1*gap,(height - 2)*gap,5*gap,2*gap);

        return 0;
    }



    public static void drawRobots(Graphics g) throws FileNotFoundException {
        boolean isShow = false;

        Font font = new Font("Times New Roman", Font.PLAIN, 9);
        g.setColor(Color.BLACK);
        g.setFont(font);

        String path = "E:\\project\\3X3V6\\video\\V0\\robots.txt";
        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();
        int num = 0;
        while (in.hasNextLine()) {
            num = in.nextInt();

            if(num == -1){
                break;
            }

            int[] arrs = {num};

            data.add(arrs);
        }

        int[] robotNode = new int[5];
        int index = 0;
        for (int i = 0; i < data.size(); i++) {
            int[] arr = (int[]) data.get(i);

            robotNode[index] = arr[0];
            index++;
            if(index == 5) {
                index = 0;
                robotNode[2]+=2;
                if(robotNode[3] > 0){
                    g.setColor(Color.MAGENTA);
                    g.fillOval(robotNode[1]*gap + gridSize/4 - 2, robotNode[2]*gap + (gridSize/4)*3 - 1, 3,3);
                    g.fillOval(robotNode[1]*gap + (gridSize/4)*3 - 2, robotNode[2]*gap + (gridSize/4)*3 - 1, 3,3);
                    g.fillRect(robotNode[1]*gap-1,robotNode[2]*gap  + gridSize/2, gridSize+1, gridSize/4);
                    if(isShow == true) {
                        g.drawString(String.valueOf(robotNode[0]), robotNode[1] * gap + gridSize + 1, robotNode[2] * gap + gridSize);
                    }

                    if(robotNode[4] > 0) {
                        int X;
                        if(robotNode[4] < 10){
                            X = robotNode[1] * gap + gridSize / 4 - 2;
                        }else{
                            X = robotNode[1] * gap + gridSize / 4 - 4;
                        }
                        g.setColor(Color.BLACK);
                        g.drawString(String.valueOf(robotNode[4]), X + 2, robotNode[2] * gap + gridSize / 2 );
                    }
                }else{
                    g.setColor(Color.blue);
                    g.fillRect(robotNode[1]*gap,robotNode[2]*gap + gridSize - 5, gridSize, 5);
                    g.fillRect(robotNode[1]*gap + gridSize/2 - 2, robotNode[2]*gap, 2, gridSize - 5);
                    int [] xPoints = new int [] {robotNode[1]*gap + gridSize/2, robotNode[1]*gap + gridSize/2 + 3, robotNode[1]*gap + gridSize/2 + 5};
                    int [] yPoints = new int [] {robotNode[2]*gap, robotNode[2]*gap + 2, robotNode[2]*gap + 2};
                    g.drawPolyline(xPoints, yPoints, 3);
                    g.drawLine(robotNode[1]*gap + gridSize/2 + 3, robotNode[2]*gap + 3, robotNode[1]*gap + gridSize/2 + 5, robotNode[2]*gap + 1);
                    g.drawLine(robotNode[1]*gap + gridSize/2 + 3, robotNode[2]*gap + 3, robotNode[1]*gap + gridSize/2 + 5, robotNode[2]*gap + 5);
                    if(isShow == true) {
                        g.drawString(String.valueOf(robotNode[0]), robotNode[1] * gap + gridSize + 1, robotNode[2] * gap + gridSize);
                    }
                }
            }
        }
    }

}



