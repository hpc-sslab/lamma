file="2-1.1x2core"
./${file}_out > ${file}_log
sleep 2
mv result/dMMRES.sum result/${file}_result
sleep 2

file="2-2.2x2core"
./${file}_out > ${file}_log
sleep 2
mv result/dMMRES.sum result/${file}_result
sleep 2

file="2-3.1x4core"
./${file}_out > ${file}_log
sleep 2
mv result/dMMRES.sum result/${file}_result
sleep 2

file="2-4.2x4core"
./${file}_out > ${file}_log
sleep 2
mv result/dMMRES.sum result/${file}_result
sleep 2

file="2-5.17x4core"
./${file}_out > ${file}_log
sleep 2
mv result/dMMRES.sum result/${file}_result
sleep 2

