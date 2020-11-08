package resource;

public class task {
    private int taskId;
    private int taskGoodsId;
    private int pickingRobotId;
    private int freightRobotId;
    private int status;
    private int process_id;
    private int task_average_period;
    private int task_average_wait_period;

    public void setValue(int p1,int p2,int p3,int p4, int p5, int p6,int p7,int p8){
        taskId = p1;
        taskGoodsId = p2;
        pickingRobotId = p3;
        freightRobotId = p4;
        status = p5;
        process_id = p6;
        task_average_period = p7;
        task_average_wait_period = p8;
    }

    public int getTaskId(){
        return taskId;
    }

    public int getTaskGoodsId(){
        return taskGoodsId;
    }

    public int getPickingRobotId(){
        return pickingRobotId;
    }

    public int getFreightRobotId(){
        return freightRobotId;
    }

    public int getTaskStatus(){
        return status;
    }

    public int getProcess_id(){
        return process_id;
    }

    public int getTask_average_period(){
        return task_average_period;
    }

    public int getTask_average_wait_period(){
        return task_average_wait_period;
    }

}
