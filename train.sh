#使用这个文件可以创建16个文件夹,同步使用粒子群算法
#使用原因:如果初始条件不够好很容易陷入局部最优
mkdir Train
cd Train
count=1
while ((count<=16))
do
mkdir Train$count
cp -p ../main ./Train$count
touch ./Train$count/trainLog
let "count++"
done