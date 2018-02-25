#!/bin/bash

CHECKER=./tau2015_checker/goldenFileChecker.pl
BENCHMARK=./benchmark/*/
RUNTIME=0
TIMER=$1

echo "Performing regression on TAU15 benchmarks ..."

OUTPUT_OFFSET=0
for i in ${BENCHMARK}
do
  TESTCASE=$(echo ${i} | sed 's/\// /g' | awk '{print $NF}')
  if [ ${#TESTCASE} -gt ${OUTPUT_OFFSET} ]; then
    OUTPUT_OFFSET=${#TESTCASE} 
  fi
done
((OUTPUT_OFFSET += 2));

for i in ${BENCHMARK}
do 
  TESTCASE=$(echo ${i} | sed 's/\// /g' | awk '{print $NF}')
  printf "Regression on %${OUTPUT_OFFSET}s" "${TESTCASE}: ";
  tar -xzf ${i}*.tar.gz --directory ${i};
  stamp_beg=`date +%s`;
	./${TIMER} ${i}*.tau2015 ${i}*.timing ${i}*.ops ${i}myout; 
  stamp_end=`date +%s`;
  ((RUNTIME += stamp_end - stamp_beg));
  ${CHECKER} ${i}*.ops ${i}*.output ${i}myout | grep 'FINAL'; 
  find ${i} -type f -not -name '*.tar.gz' | xargs rm -rf; 
done

echo "Regression runtime: ${RUNTIME} seconds"


