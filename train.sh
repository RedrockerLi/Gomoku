#使用这个文件可以创建16个文件夹,同步使用粒子群算法
#使用原因:如果初始条件不够好很容易陷入局部最优
./build.sh
rm -rf Train
mkdir Train
cd Train
count=1
while ((count<=16))
do
sleep 1
mkdir Train$count
cp -p ../main ./Train$count
touch ./Train$count/trainLog
cd ./Train$count
./main &
cd ..
let "count++"
done

#结束任务
# count=1
# cd ./Train
# while ((count<=16))
# do
# cd ./Train$count
# killall ./main
# cd ..
# let "count++"
# done