import resource.task;

import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.Vector;


public class Map extends Thread {
    public static int nodeId = 0;
    public static int goodsID = 0;
    public static int start = 0, gap = 11;
    public static int gridSize = 10;
    public static int shelf_w = 16, shelf_h = 11, goods_w = 12, goods_h = 2, road1 = 2, road2 = 3;
    public static int width = 76, height = 47;
    public static int shelfXnum = 3, shelfYnum = 3;
    public static int start_x = 4, start_y = 4;

    public static int taskAveragePeriod = 0;
    public static int taskWaitAveragePeriod = 0;


    private JFrame jFrame = new JFrame();

    public static int pickingRobotNum = 0;
    public static int fixFreightRobotNum = 0;
    public static int freeFreightRobotNum = 0;
    public static int freightRobotNum = 0;
    public static int allRobotNum = 0;

    private Integer robotId = 0;

    public static Integer[][] robotLocation = new Integer[allRobotNum][2];

    public static int count = 0;

    public static int goodsTypeNum = 9;

    public static int periodId = 0;

    @Override
    public void run() {
        jFrame.getContentPane().setBackground(Color.white);         //设置背景颜色
        jFrame.setSize(850, 555);                    //1500X900显示画框
        jFrame.setTitle("抓取-搬运机器人协作调度仿真系统");

        JPanel jp1 = new JPanel() {
            public void paint(Graphics g) {
                super.paint(g);

                try {
                    drawMap(g);
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }

                drawTarget(g);                                     //把目标位置写入数据库

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

            jp1 = new JPanel() {                          //重绘jpanel，实现界面刷新
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

        String path = "E:\\project\\test\\map1.txt";
        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();
        while (in.hasNextLine()) {
            int num = in.nextInt();

            if (num == -1) {
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
            if (index == 5) {
                if (mapNode[3] == 0) {
                    g.setColor(Color.BLACK);
                } else if (mapNode[3] == 1) {
                    g.setColor(Color.LIGHT_GRAY);
                } else if (mapNode[3] == 2) {
                    g.setColor(Color.ORANGE);
                } else if (mapNode[3] == 3) {
                    g.setColor(Color.red);
                }
                g.fillRect(mapNode[1] * gap, mapNode[2] * gap, gridSize, gridSize);
                index = 0;
            }

        }

        return 0;
    }


    /**
     * @param g
     * @param sql
     * @return nodeID
     */
    public static int drawTarget(Graphics g) {
        int target_x1 = width - 6, target_x2 = start + 5, target_y = start + 3;
        int targets = 0;
        int col, row;

        g.setColor(Color.white);

        row = target_y;

        g.fillRect((width - 6) * gap, (start + 3) * gap, 5 * gap, 2 * gap);
        g.fillRect((width - 6) * gap, (start + 5) * gap, 2 * gap, (height - 10) * gap);
        g.fillRect((width - 6) * gap, (height - 5) * gap, 5 * gap, 2 * gap);

        g.setColor(Color.BLACK);
        Font font = new Font("宋体", Font.BOLD, 18);
        g.setFont(font);

        int y = 10, grid = 10;
        g.drawString("传", (width - 6) * gap + 3, y * gap);
        g.drawString("送", (width - 6) * gap + 3, (y + grid) * gap);
        g.drawString("带", (width - 6) * gap + 3, (y + 2 * grid) * gap);

        nodeId += targets;

        return 0;
    }


    public static void drawRobots(Graphics g) throws FileNotFoundException {
        boolean isShow = true;

        Font font = new Font("宋体", Font.BOLD, 9);
        g.setColor(Color.BLACK);
        g.setFont(font);

        String path = "E:\\project\\test\\robots.txt";
        Scanner in = new Scanner(new File(path));
        List data = new ArrayList();
        int num = 0;
        while (in.hasNextLine()) {
            num = in.nextInt();

            if (num == -1) {
                break;
            }

            int[] arrs = {num};

            data.add(arrs);
        }

        int[] robotNode = new int[7];
        int index = 0;
        for (int i = 0; i < data.size(); i++) {
            int[] arr = (int[]) data.get(i);
            //     System.out.println(arr[0]);

            robotNode[index] = arr[0];
            index++;
            if (index == 7) {
                index = 0;
                if (robotNode[3] > 0) {
                    g.setColor(Color.MAGENTA);
                    g.fillOval(robotNode[1] * gap + gridSize / 4 - 1, robotNode[2] * gap + (gridSize / 4) * 3, 3, 3);
                    g.fillOval(robotNode[1] * gap + (gridSize / 4) * 3 - 1, robotNode[2] * gap + (gridSize / 4) * 3, 3, 3);
                    g.fillRect(robotNode[1] * gap, robotNode[2] * gap + gridSize / 2, gridSize, gridSize / 4);
                    if (isShow == true) {
                        g.drawString(String.valueOf(robotNode[0]), robotNode[1] * gap + gridSize + 1, robotNode[2] * gap + gridSize);
                    }

                    if (robotNode[4] > 0) {
                        //   g.drawRect(resultSet[i][0] * gap + gridSize / 4 - 3,resultSet[i][1] * gap - 4, 12,8);
                        int X;
                        if (robotNode[4] < 10) {
                            X = robotNode[1] * gap + gridSize / 4 - 2;
                        } else {
                            X = robotNode[1] * gap + gridSize / 4 - 4;
                        }
                        g.setColor(Color.BLACK);
                        g.drawString(String.valueOf(robotNode[4]), X + 2, robotNode[2] * gap + gridSize / 2 - 1);
                    }
                } else {
                    g.setColor(Color.blue);
                    g.fillRect(robotNode[1] * gap, robotNode[2] * gap + gridSize - 5, gridSize, 5);
                    g.fillRect(robotNode[1] * gap + gridSize / 2 - 2, robotNode[2] * gap, 2, gridSize - 5);
                    int[] xPoints = new int[]{robotNode[1] * gap + gridSize / 2, robotNode[1] * gap + gridSize / 2 + 3, robotNode[1] * gap + gridSize / 2 + 5};
                    int[] yPoints = new int[]{robotNode[2] * gap, robotNode[2] * gap + 2, robotNode[2] * gap + 2};
                    g.drawPolyline(xPoints, yPoints, 3);
                    g.drawLine(robotNode[1] * gap + gridSize / 2 + 3, robotNode[2] * gap + 3, robotNode[1] * gap + gridSize / 2 + 5, robotNode[2] * gap + 1);
                    g.drawLine(robotNode[1] * gap + gridSize / 2 + 3, robotNode[2] * gap + 3, robotNode[1] * gap + gridSize / 2 + 5, robotNode[2] * gap + 5);
                    if (isShow == true) {
                        g.drawString(String.valueOf(robotNode[0]), robotNode[1] * gap + gridSize + 1, robotNode[2] * gap + gridSize);
                    }

                }
            }
        }
    }

}



