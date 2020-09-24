#!/bin/bash

exe=./Janis

dir=/global/home/users/suerfu/LHe/janis/sim/build
outdir=/global/home/users/suerfu/data/janis-sim

while [ $(squeue -u suerfu | grep jn | wc -l) -ne 0 ]
do
    sleep 300;
done

for i in {1..30}
do
    for j in {1..2}
    do
        sbfile=.temp.txt
        script=neutron${j}.mac

        echo "#!/bin/bash" > $sbfile

        echo "#SBATCH --job-name=jn_${i}" >> $sbfile
        echo "#SBATCH --account=fc_hedm" >> $sbfile
        echo "#SBATCH --partition=savio2_htc" >> $sbfile # savio2_htc
        echo "#SBATCH --qos=savio_normal" >> $sbfile

        echo "#SBATCH --nodes=1" >> $sbfile
        echo "#SBATCH --ntasks=1" >> $sbfile
#        echo "#SBATCH --ntasks-per-node=12" >> $sbfile
        echo "#SBATCH --cpus-per-task=1" >> $sbfile

        echo "#SBATCH --mem=5000M" >> $sbfile
        echo "#SBATCH --time=23:59:00" >> $sbfile

        if [ $i -eq 1 ]
        then
            echo "#SBATCH --mail-user=suerfu@berkeley.edu" >> $sbfile
            echo "#SBATCH --mail-type=BEGIN" >> $sbfile
            echo "#SBATCH --mail-type=END" >> $sbfile
        fi

        echo "cd $dir" >> $sbfile
        echo "$exe -m $script -f ${outdir}/mc_neutron_config${j}_run${i}.root" >> $sbfile
        #echo "./Janis-node.sh" >> $sbfile

        NPrevJobs=$(squeue -u suerfu -o %T | grep RUNNING | wc -l)

        sbatch $sbfile
        rm $sbfile

        NCurJobs=$(squeue -u suerfu -o %T | grep RUNNING | wc -l)
        while [ $NCurJobs -eq $NPrevJobs ]
        do
            sleep 1
            NCurJobs=$(squeue -u suerfu -o %T | grep RUNNING | wc -l)
        done
        sleep 1
        echo "    " $NCurJobs " running..."
    done
done

