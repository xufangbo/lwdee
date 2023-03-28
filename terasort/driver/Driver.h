#pragma once

#include <iostream>
#include <vector>

#include "core/Bytes10.h"
#include "core/Partition.h"
#include "lwdee/DCO.h"
 #include <sys/time.h>

typedef std::pair<Bytes10, Bytes10> MinAndMax;

class Driver {
 private:
  std::string inputFile;                  // 数据源文件
  std::string outputFile;                 // 数据源文件
  int datum;                              // 采样基准
  int splitNums1;                         // map 计算分区数
  int splitNums2;                         // reduce计算分区数
  std::vector<SampleSplit> sampleSplits;  // 样本分割
  std::vector<pair<DCO, DDOId>> step1Invokers;
  std::vector<pair<DCO, DDOId>> step2Invokers;
  std::vector<PartitionStep2> step2Inputs;

 public:
  void startJob(std::string inputFile, std::string outputFile, int datum, int splitNum1, int splitNum2);

 private:
  /**
   * 采样得到最大值最小值
   * */
  MinAndMax samples(std::string fileName);

  /**
   * 基于样本最大值最小值进行区间分割
   * */
  void split(MinAndMax conf);

  /**
   * 执行map计算
   * */
  void map();

  void mapToReduce();

  /**
   * 执行reduce计算
   * */
  void reduce();

 private:
  struct timeval startTs;
  void startWatch();
  void stopWath();
};