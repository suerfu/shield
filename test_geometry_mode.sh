file1=test_nested3_3.txt
file2=test_standalone3_3.txt

comment="# 10 cm vacuum, Si-C-Si-C-He-Si-He-Si, each 5 cm, 100000000 2-MeV gamams." 
echo >> $file1
echo >> $file2

for i in {1..100}
do
    ( time ./shield -m test.mac ) |& tail -3 >> $file1
    ( time ./shield -m test.mac --standalone ) |& tail -3 >> $file2
done

