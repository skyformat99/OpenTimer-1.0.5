#!/usr/bin/perl -w
use FileHandle;
use List::Util qw[min max];

# usage: <.ops file> <golden output file> <contestant output file> [flags]

# always compare golden with contestant: if not present in contestant, then no credit

# When generating contestants, allow -numPaths to be  #
# greater than what we're actually evaluating, e.g.,  #
# by 10% or so.                                       #
#                                                     #
# ========================= Value Accuracy Evaluation ================================ #
# For each rat/at/slack:                                                               #
#   abs(diff) ~ [0.0, $accuracyThresholdH]                    ps => $accuracyScoreH    #
#   abs(diff) ~ ($accuracyThresholdH, $accuracyThresholdM]    ps => $accuracyScoreM    #
#   abs(diff) ~ ($accuracyThresholdM, $accuracyThresholdL]    ps => $accuracyScoreL    #
#   abs(diff) ~ ($accuracyThresholdL, \infinity)              ps => $accuracyScoreZero #
# Note: Full credit if both slack values are positive                                  #
#-----------------------------------------------------#
# For each PATH: match full path. If path found, then match slack as above criteria.   #
#   if path found, then match slack                   #
#   75% + 25% for slack (same critieria as above)     #
# Full credit if both slack values are positive       #
#------------------------------------------------#
# Overall metrics:                                                     #
#  a) average all report_at/slack/rat                                  #
#  b) report_worst_paths -numPaths 100                                 #
#  c) average of report_worst_paths -pin <every test pin> -numPaths 10 #
#  d) worst path accuracy score (discard one 0?) #
#------------------------------------------------#

local $verbosePrintPaths = 0;
local $verbosePrintValues = 0;
local $printVerbosePathAccuracy = 0;
local $printVerboseValueAccuracy = 0;

# add in line # from golden and contestant

local $accuracyThresholdH = 0.1;
local $accuracyThresholdM = 0.5;
local $accuracyThresholdL = 1.0;

local $accuracyScoreH     = 1;
local $accuracyScoreM     = 0.8;
local $accuracyScoreL     = 0.5;
local $accuracyScoreZero  = 0;


local $pathScoreH             = 1;
local $pathVSlackPathPortion  = 0.75;
local $pathVSlackSlackPortion = 0.25;

# key is <ops line #> <golden line #> <contestant line #>
# value is contestant accuracy score
local %valueScores = ();

# key is <ops line #> <golden line #> <contestant line #>
# value is <max score> <contestant score>
local %pathScores = ();

# key is <pathType> <mode> <path: node transition node transition...>
# value is slack
# temporary storage - will be cleared each time report_worst_paths is called
local %pathsG = ();
local %pathsC = ();

# used for verbose output only
local $countValuesH    = 0;
local $countValuesM    = 0;
local $countValuesL    = 0;
local $countValuesZero = 0;

local %valuesZero      = ();
local %valuesL         = ();
local %valuesM         = ();

local %reportCommands  = ();

local %pathsGoldenMissing = ();
local %pathsSlacksM = ();
local %pathsSlacksL = ();
local %pathsSlacksZero = ();

# assumes (1) golden and (2) contestant
sub absDiff_two_values
{
  unless (scalar(@_) == 2) {
    die ("requires (1) golden value and (2) contestant value\n");
  }

  my $valueGolden     = $_[0];
  my $valueContestant = $_[1];
  return abs($valueGolden - $valueContestant);
}

sub report_value_accuracy
{
  unless (scalar(@_) == 3) {
    die ("report_value_accuracy: requires (1) golden value, (2) contestant value, and (3) isSlackValue (true/false)\n");
  }

  local $valueGolden     = $_[0];
  local $valueContestant = $_[1];
  local $isSlackValue    = $_[2];

  # if comparing slack and both are positive, then we do not consider accuracy
  if ($isSlackValue && $valueGolden >= 0 && $valueContestant >= 0) {
    return $accuracyScoreH;
  }

  local $valueDiff = &absDiff_two_values($valueGolden, $valueContestant);
  if ($valueDiff <= $accuracyThresholdH) {
    return $accuracyScoreH;
  }
  elsif ($valueDiff <= $accuracyThresholdM) {
    return $accuracyScoreM;
  }
  elsif ($valueDiff <= $accuracyThresholdL) {
    return $accuracyScoreL;
  }
  else {
    return $accuracyScoreZero;
  }
}

# value is <max score> <contestant score>
sub report_path_accuracy
{
  local $lineCountO;
  local $lineO;

  if ($printVerbosePathAccuracy) {
    unless (scalar (@_) == 2) {
      die ("report_path_accuracy: requires two arguments -- (1) ops line # and (2) ops line\n");
    }
    $lineCountO = $_[0];
    $lineO      = $_[1];
  }

  local $numPathsG = scalar (keys %pathsG);
  local $goldenAccuracy = $numPathsG * $accuracyScoreH;

#  local $numPathsC = scalar (keys %pathsC);
  local $contestantAccuracy = 0;

  foreach $pathGolden (keys %pathsG) {
    if (exists $pathsC{$pathGolden}) {
      local $isSlackValue = 1;
      local @pathGInfo = split(' ', $pathsG{$pathGolden});
      local $pathGSlack = $pathGInfo[0];
      local $pathGNumber = $pathGInfo[1];
      local @pathCInfo = split(' ', $pathsC{$pathGolden});
      local $pathCSlack = $pathCInfo[0];
      local $pathCNumber = $pathCInfo[1];
      local $valueAccuracy = &report_value_accuracy($pathGSlack, $pathCSlack, $isSlackValue);
      $contestantAccuracy += $pathScoreH*$pathVSlackPathPortion + $valueAccuracy*$pathVSlackSlackPortion;
      if ($printVerbosePathAccuracy) {
        if ($valueAccuracy == $accuracyScoreM) {
          $reportCommands{$lineCountO} = $lineO;
          local $badSlackPathKey = "$lineCountO $pathGolden";
          $pathsSlacksM{$badSlackPathKey} = "$pathGSlack $pathGNumber $pathCSlack $pathCNumber";
        }
        elsif ($valueAccuracy == $accuracyScoreL) {
          $reportCommands{$lineCountO} = $lineO;
          local $badSlackPathKey = "$lineCountO $pathGolden";
          $pathsSlacksL{$badSlackPathKey} = "$pathGSlack $pathGNumber $pathCSlack $pathCNumber";
        }
        elsif ($valueAccuracy == $accuracyScoreZero) {
          $reportCommands{$lineCountO} = $lineO;
          local $badSlackPathKey = "$lineCountO $pathGolden";
          $pathsSlacksZero{$badSlackPathKey} = "$pathGSlack $pathGNumber $pathCSlack $pathCNumber";
#          print "Jinhu: slacks are $pathsSlacksZero{$badSlackPathKey}\n";
        }
      }
    }
    elsif ($printVerbosePathAccuracy) {
      $reportCommands{$lineCountO} = $lineO;
      local $missingPathKey = "$lineCountO $pathGolden";
      local @pathGInfo = split(' ', $pathsG{$pathGolden});
      local $pathGSlack = $pathGInfo[0];
      local $pathGNumber = $pathGInfo[1];
      $pathsGoldenMissing{$missingPathKey} = "$pathGSlack $pathGNumber";
    }
  }

  return "$goldenAccuracy $contestantAccuracy";

}

sub report_values_accuracy
{
  local $totalAccuracy = 0;
  local $totalQueries = scalar (keys %valueScores);
  foreach $valueKey (keys %valueScores) {
    $totalAccuracy += $valueScores{$valueKey};
  }

  local $average = $totalAccuracy / $totalQueries;
  print "======================================================================\n";
  printf "VALUE ACCURACY: Average of %d data points is %2.5f (raw sum = %d)\n", $totalQueries, $average*100, $totalAccuracy;
  print "======================================================================\n";
  if ($printVerboseValueAccuracy) {
    print  "===================================================================\n";
    printf "High accuracy: %d of %d (%2.5f)\n", $countValuesH, $totalQueries, $countValuesH / $totalQueries * 100.0;
    printf "Mid  accuracy: %d of %d (%2.5f)\n", $countValuesM, $totalQueries, $countValuesM / $totalQueries * 100.0;
    printf "Low  accuracy: %d of %d (%2.5f)\n", $countValuesL, $totalQueries, $countValuesL / $totalQueries * 100.0;
    printf "No accuracy:   %d of %d (%2.5f)\n", $countValuesZero, $totalQueries, $countValuesZero / $totalQueries * 100.0;
    print  "===================================================================\n";

    if ($countValuesM > 0) {
      print "VALUE MISMATCH MEDIUM: Mid-accuracy values: rat / at / slack that are between ($accuracyThresholdH, $accuracyThresholdM] picoseconds\n";
      foreach $valueKey (sort keys %valuesM) {
        local @lineCounts = split (' ', $valueKey);
        local $lineCountO = $lineCounts[0];
        local $lineCountG = $lineCounts[1];
        local $lineCountC = $lineCounts[2];
        local @values     = split (' ', $valuesM{$valueKey});
        local $valueG     = $values[0];
        local $valueC     = $values[1];
        printf "Ops [$lineCountO]:: $reportCommands{$lineCountO} :: Golden [$lineCountG] = $valueG vs. Contestant [$lineCountC] = $valueC (diff = %2.5f)\n", &absDiff_two_values($valueG, $valueC);
      }
      print  "-----------------------------------------------------------------\n";
    }
    if (scalar (keys %valuesL) > 0) {
      print "VALUE MISMATCH LOW: Low-accuracy values: rat / at / slack that are between ($accuracyThresholdM, $accuracyThresholdL] picoseconds\n";
      foreach $valueKey (sort keys %valuesL) {
        local @lineCounts = split (' ', $valueKey);
        local $lineCountO = $lineCounts[0];
        local $lineCountG = $lineCounts[1];
        local $lineCountC = $lineCounts[2];
        local @values     = split (' ', $valuesL{$valueKey});
        local $valueG     = $values[0];
        local $valueC     = $values[1];
        printf "Ops [$lineCountO]:: $reportCommands{$lineCountO} :: Golden [$lineCountG] = $valueG vs. Contestant [$lineCountC] = $valueC (diff = %2.5f)\n", &absDiff_two_values($valueG, $valueC);
      }
      print  "-----------------------------------------------------------------\n";
    }
    if (scalar (keys %valuesZero) > 0) {
      print "VALUE MISMATCH ZERO: No-accuracy values: rat / at / slack that are between ($accuracyThresholdL, \\infinity) picoseconds\n";
      foreach $valueKey (sort keys %valuesZero) {
        local @lineCounts = split (' ', $valueKey);
        local $lineCountO = $lineCounts[0];
        local $lineCountG = $lineCounts[1];
        local $lineCountC = $lineCounts[2];
        local @values     = split (' ', $valuesZero{$valueKey});
        local $valueG     = $values[0];
        local $valueC     = $values[1];
        printf "Ops [$lineCountO]:: $reportCommands{$lineCountO} :: Golden [$lineCountG] = $valueG vs. Contestant [$lineCountC] = $valueC (diff = %2.5f)\n", &absDiff_two_values($valueG, $valueC);
      }
      print  "-----------------------------------------------------------------\n";
    }
    if ($countValuesZero + $countValuesH + $countValuesM + $countValuesL != $totalQueries) {
      die ("ERROR: Total Count does not equal totalQueries...\n");
    }
  }
  return $average;
}

sub report_paths_accuracy
{
  local $maxScore = 0;
  local $contestantScore = 0;
  foreach $pathKey (keys %pathScores) {
    local @scores = split(' ', $pathScores{$pathKey});
    local $currentMaxScore        = $scores[0];
    local $currentContestantScore = $scores[1];
    $maxScore += $currentMaxScore;
    $contestantScore += $currentContestantScore;
  }
  local $average = $contestantScore / $maxScore;
  print  "======================================================================\n";
  printf "PATH ACCURACY: Average of %d paths is %3.5f (raw sum of %d)\n", $maxScore, $average*100., $contestantScore;
  print  "======================================================================\n";
  if ($printVerbosePathAccuracy) {
    if (scalar (keys %pathsSlacksM) > 0) {
      print "------------------------------------------------------------------\n";
      print "PATH SLACK MISMATCH MEDIUM: Path slack difference mismatch between ($accuracyThresholdH, $accuracyThresholdM] picoseconds\n";
      foreach $pathSlackKey (sort keys %pathsSlacksM) {
        $pathSlackKey =~ /^([0-9]+) (.+)/;
        local $lineCountO = $1;
        local $path       = $2;
        local @slacks = split(' ', $pathsSlacksM{$pathSlackKey});
        local $slackGolden = $slacks[0];
        local $numberGolden = $slacks[1];
        local $slackContestant = $slacks[2];
        local $numberContestant = $slacks[3];
        printf "Ops line [$lineCountO]:: $reportCommands{$lineCountO} :: golden path [%4d] with slack = $slackGolden vs. contestant path [%4d] with slack = $slackContestant (diff = %2.5f) [path follows] <type> <mode> <node transition>)\n", $numberGolden, $numberContestant, &absDiff_two_values($slackGolden, $slackContestant);
        print "--> $path\n";
      }
      print "------------------------------------------------------------------\n";
    }
    if (scalar (keys %pathsSlacksL) > 0) {
      print "------------------------------------------------------------------\n";
      print "PATH SLACK MISMATCH LOW: Path slack difference mismatch between ($accuracyThresholdM, $accuracyThresholdL] picoseconds\n";
      foreach $pathSlackKey (sort keys %pathsSlacksL) {
        $pathSlackKey =~ /^([0-9]+) (.+)/;
        local $lineCountO = $1;
        local $path       = $2;
        local @slacks = split(' ', $pathsSlacksL{$pathSlackKey});
        local $slackGolden = $slacks[0];
        local $numberGolden = $slacks[1];
        local $slackContestant = $slacks[2];
        local $numberContestant = $slacks[3];
        printf "Ops line [$lineCountO]:: $reportCommands{$lineCountO} :: golden path [%4d] with slack = $slackGolden vs. contestant path [%4d] with slack = $slackContestant (diff = %2.5f) [path follows] <type> <mode> <node transition>)\n", $numberGolden, $numberContestant, &absDiff_two_values($slackGolden, $slackContestant);
        print "--> $path\n";
      }
      print "------------------------------------------------------------------\n";
    }
    if (scalar (keys %pathsSlacksZero) > 0) {
      print "------------------------------------------------------------------\n";
      print "PATH SLACK MISMATCH ZERO: Path slack difference mismatch between ($accuracyThresholdL, \\infinity) picoseconds\n";
      foreach $pathSlackKey (sort keys %pathsSlacksZero) {
        $pathSlackKey =~ /^([0-9]+) (.+)/;
        local $lineCountO = $1;
        local $path       = $2;
        local @slacks = split(' ', $pathsSlacksZero{$pathSlackKey});
        local $slackGolden = $slacks[0];
        local $numberGolden = $slacks[1];
        local $slackContestant = $slacks[2];
        local $numberContestant = $slacks[3];
        printf "Ops line [$lineCountO]:: $reportCommands{$lineCountO} :: golden path [%4d] with slack = $slackGolden vs. contestant path [%4d] with slack = $slackContestant (diff = %2.5f) [path follows] <type> <mode> <node transition>)\n", $numberGolden, $numberContestant, &absDiff_two_values($slackGolden, $slackContestant);
        print "--> $path\n";
      }
      print "------------------------------------------------------------------\n";
    }
    if (scalar (keys %pathsGoldenMissing) > 0) {
      print "------------------------------------------------------------------\n";
      print "MISSING PATHS: Contestant file is missing the following paths that are in golden:\n";
      foreach $missingPath (sort keys %pathsGoldenMissing) {
        $missingPath =~ /^([0-9]+) (.+)/;
        local $lineCountO = $1;
        local $path       = $2;
        local @pathGInfo = split(' ', $pathsGoldenMissing{$missingPath});
        local $pathGSlack = $pathGInfo[0];
        local $pathGNumber = $pathGInfo[1];
        printf "Ops line [$lineCountO]:: $reportCommands{$lineCountO} :: missing golden path [%4d] with slack $pathGSlack (<type> <mode> <node transition>)\n", $pathGNumber;
        print "--> $path\n";
      }
      print "------------------------------------------------------------------\n";
    }
  }
  return $average;
}

sub parse_value
{
  unless (scalar (@_) == 2) {
    die ("parse_value: requires two arguments --- file handle and line count\n");
  }

  local $FILEIN    = $_[0];
  local $lineCount = $_[1];
  local $line = <FILEIN>;
  
  chomp ($line);
  $lineCount++;

  if ($line =~ /([0-9e\-\.\+]+)/) {
    local $value = $1;

    return "$value $lineCount";
  }
  else {
    print "lineCount = $lineCount, line = $line\n";
    die ("ERROR: unknown line syntax\n");
  }
}

sub store_verbose_values
{
  unless (scalar (@_) == 4) {
    die ("store_verbose_values: requires three arguments -- (1) value key, (2) command line, (3) golden value, (4) contestant value\n");
  }
  local $valueKey = $_[0];
  local $command  = $_[1];
  local $valueG   = $_[2];
  local $valueC   = $_[3];

#  print "valueKey = $valueKey\n";

  if ($valueScores{$valueKey} == $accuracyScoreH) {
    $countValuesH++;
    return;
  }

  local @lineCounts = split(' ', $valueKey);
  local $lineCountO = $lineCounts[0];
  $reportCommands{$lineCountO} = $command;

  local $values = "$valueG $valueC";

#  print "values = $values\n";
  if ($valueScores{$valueKey} == $accuracyScoreM) {
    $valuesM{$valueKey} = $values;
    $countValuesM++;
  }
  elsif ($valueScores{$valueKey} == $accuracyScoreL) {
    $valuesL{$valueKey} = $values;
    $countValuesL++;
  }
  elsif ($valueScores{$valueKey} == $accuracyScoreZero) {
    $valuesZero{$valueKey} = $values;
    $countValuesZero++;
  }
}

sub parse_ops_and_output_files
{
  unless (scalar (@_) == 3) {
    die ("parse_ops_and_output_files: requires three arguments -- (1) ops file, (2) golden and (3) contestant output\n");
  }
  local $opsFile        = $_[0];
  local $goldenFile     = $_[1];
  local $contestantFile = $_[2];

  local *FILEINO;
  local *FILEING;
  local *FILEINC;
  
  if ($opsFile =~ /([^\s]+)\.gz/) {
    open (FILEINO, "gunzip -c $opsFile |") or die ("# ERROR::parse_ops_and_output_files:: cannot open ops file $opsFile\n");
  }
  else {
    open (FILEINO, "$opsFile") or die ("# ERROR::parse_ops_and_output_files:: cannot open ops $opsFile\n");
  }
  
  if ($goldenFile =~ /([^\s]+)\.gz/) {
    open (FILEING, "gunzip -c $goldenFile |") or die ("# ERROR::parse_ops_and_output_files:: cannot open golden file $goldenFile\n");
  }
  else {
    open (FILEING, "$goldenFile") or die ("# ERROR::parse_ops_and_output_files:: cannot open golden $goldenFile\n");
  }
  
  if ($contestantFile =~ /([^\s]+)\.gz/) {
    open (FILEINC, "gunzip -c $contestantFile |") or die ("# ERROR::parse_ops_and_output_files:: cannot open contestant file $contestantFile\n");
  }
  else {
    open (FILEINC, "$contestantFile") or die ("# ERROR::parse_ops_and_output_files:: cannot open contestant file $contestantFile\n");
  }
  
  local $lineCountO = 0;
  local $lineCountG = 0;
  local $lineCountC = 0;
  local $lineO;
  local $lineG;
  local $lineC;

  local $isSlackValue;
  
  while ($lineO = <FILEINO>)
  {
    chomp ($lineO);
    $lineCountO++;
#    print "lineCountO = $lineCountO, lineO = $lineO\n";

    if ($lineO =~ /^report_at/) {
      $isSlackValue = 0;

      local $atValueG;
      local $atValueC;

      # golden AT value
      if ($lineG = <FILEING>) {
        chomp ($lineG);
        $lineCountG++;
        if ($lineG =~ /([0-9e\-\.\+]+)/) {
          $atValueG = $1;
        }
        else {
          die ("ERROR: unknown line syntax in golden report_at at lineCountG = $lineCountG, lineG = $lineG (ops file [$lineCountO], lineO = $lineO)\n");
        }
      }
      else {
        print "lineG = $lineG\n";
        die ("ERROR: error reading line in golden for report_at at lineCountG = $lineCountG, lineG = $lineG (ops file [$lineCountO], lineO = $lineO)\n");
      }
      
      # contestant AT value
      if ($lineC = <FILEINC>) {
        chomp ($lineC);
        $lineCountC++;
#        print "lineCountC = $lineCountC, lineC = $lineC\n";
        if ($lineC =~ /([0-9e\-\.\+]+)/) {
          $atValueC = $1;
        }
        else {
          die ("ERROR: unknown line syntax in contestant report_at at lineCountG = $lineCountC, lineG = $lineC (ops file [$lineCountO], lineO = $lineO)\n");
        }
      }
      else {
        print "lineC = $lineC\n";
        die ("ERROR: error reading line in contestant for report_at at line $lineCountC (ops file [$lineCountO], lineO = $lineO)\n");
      }

      if ($verbosePrintValues) {
        print "Ops line [$lineCountO]:: $lineO :: Golden line [$lineCountG] = $atValueG vs. Contestant line [$lineCountC] = $atValueC\n";
      }

      local $valueKey = "$lineCountO $lineCountG $lineCountC";
      $valueScores{$valueKey} = &report_value_accuracy($atValueG, $atValueC, $isSlackValue);
      if ($printVerboseValueAccuracy) {
        &store_verbose_values($valueKey, $lineO, $atValueG, $atValueC);
      }
    }
    elsif ($lineO =~ /^report_rat/) {
      $isSlackValue = 0;

      local $ratValueG;
      local $ratValueC;

      # golden RAT value
      if ($lineG = <FILEING>) {
        chomp ($lineG);
        $lineCountG++;
        if ($lineG =~ /([0-9e\-\.\+]+)/) {
          $ratValueG = $1;
        }
        else {
          die ("ERROR: unknown line syntax in golden report_rat at lineCountG = $lineCountG, lineG = $lineG (ops file [$lineCountO], lineO = $lineO)\n");
        }
      }
      else {
        die ("ERROR: error reading line in golden for report_rat at lineCountG = $lineCountG, lineG = $lineG (ops file [$lineCountO], lineO = $lineO)\n");
      }
      
      # contestant RAT value
      if ($lineC = <FILEINC>) {
        chomp ($lineC);
        $lineCountC++;
#        print "lineCountC = $lineCountC, lineC = $lineC\n";
        if ($lineC =~ /([0-9e\-\.\+]+)/) {
          $ratValueC = $1;
        }
        else {
          die ("ERROR: unknown line syntax in contestant report_rat at lineCountC = $lineCountC, lineC = $lineC (ops file [$lineCountO], lineO = $lineO)\n");
        }
      }
      else {
        die ("ERROR: error reading line in contestant for report_rat at lineCountC = $lineCountC, lineC = $lineC (ops file [$lineCountO], lineO = $lineO)\n");
      }
      
      if ($verbosePrintValues) {
        print "Ops line [$lineCountO]:: $lineO :: Golden line [$lineCountG] = $ratValueG vs. Contestant line [$lineCountC] = $ratValueC\n";
      }

      local $valueKey = "$lineCountO $lineCountG $lineCountC";
      $valueScores{$valueKey} = &report_value_accuracy($ratValueG, $ratValueC, $isSlackValue);
      if ($printVerboseValueAccuracy) {
        &store_verbose_values($valueKey, $lineO, $ratValueG, $ratValueC);
      }
    }
    elsif ($lineO =~ /^report_slack/) {
      $isSlackValue = 1;

      local $slackValueG;
      local $slackValueC;

      # golden slack value
      if ($lineG = <FILEING>) {
        chomp ($lineG);
        $lineCountG++;
        if ($lineG =~ /([0-9e\-\.\+]+)/) {
          $slackValueG = $1;
        }
        else {
          die ("ERROR: unknown line syntax in golden report_slack at lineCountG = $lineCountG, lineG = $lineG (ops file [$lineCountO], lineO = $lineO)\n");
        }
      }
      else {
        die ("ERROR: error reading line in golden for report_slack at lineCountG = $lineCountG, lineG = $lineG (ops file [$lineCountO], lineO = $lineO)\n");
      }
      
      # contestant slack value
      if ($lineC = <FILEINC>) {
        chomp ($lineC);
        $lineCountC++;
#        print "lineCountC = $lineCountC, lineC = $lineC\n";
        if ($lineC =~ /([0-9e\-\.\+]+)/) {
          $slackValueC = $1;
        }
        else {
          die ("ERROR: unknown line syntax in contestant report_slack at lineCountC = $lineCountC, lineC = $lineC (ops file [$lineCountO], lineO = $lineO)\n");
        }
      }
      else {
        die ("ERROR: error reading line in contestant for report_slack at lineCountC $lineCountC, lineC = $lineC (ops file [$lineCountO], lineO = $lineO)\n");
      }
      
      if ($verbosePrintValues) {
        print "Ops line [$lineCountO]:: $lineO :: Golden line [$lineCountG] = $slackValueG vs. Contestant line [$lineCountC] = $slackValueC\n";
      }

      local $valueKey = "$lineCountO $lineCountG $lineCountC";
      $valueScores{$valueKey} = &report_value_accuracy($slackValueG, $slackValueC, $isSlackValue);
      if ($printVerboseValueAccuracy) {
        &store_verbose_values($valueKey, $lineO, $slackValueG, $slackValueC);
      }
    }
    elsif ($lineO =~ /^report_worst_paths/) {
      $isSlackValue = 1;
      local $pathKey = "$lineCountO";
      
      local $pathNumber;
      local $pathType;
      local $pathSlack;
      local $pathMode;
      local $pathLength;
      local $pathLengthCounter;
      local $path;

      local $numPaths;

      $pathKey .= " $lineCountG";
      
      # populates golden paths
      %pathsG = ();

      while ($lineG = <FILEING>) {
        $lineCountG++;
#        print "lineCountG = $lineCountG, lineG = $lineG\n";
        # key is <pathType> <mode> <path: node transition node transition...>
        if ($lineG =~ /report_worst_paths ([0-9]+)/) {
          $numPaths = $1;
          # no golden paths
          if ($numPaths == 0) {
            last;
          }
        }
        elsif ($lineG =~ /Path ([0-9]+): ([^\s]+) ([0-9e\-\.\+]+) ([0-9]+) ([EL])/) {
          $pathNumber = $1;
          $pathType   = $2;
          $pathSlack  = $3;
          $pathLength = $4;
          $pathMode   = $5;
          $path       = "$pathType $pathMode";
#          print "Processing Path $pathNumber of $numPaths, with type = $pathType, length = $pathLength, slack = $pathSlack, mode = $pathMode\n";
          $pathLengthCounter = 0;
        }
        elsif ($lineG =~ /([^\s]+) ([RF])/) {
          local $pathNode       = $1;
          local $pathTransition = $2;
          $pathLengthCounter++;
#          print "Current path node at $pathNode with transition $pathTransition\n";
          $path .= " $pathNode $pathTransition";
          if ($pathLengthCounter == $pathLength) {
            $pathsG{$path} = "$pathSlack $pathNumber";
            if ($pathNumber == $numPaths) {
              last;
            }
          }
        }
        else {
          die ("ERROR: unknown construct in golden report_worst_paths, lineCountG = $lineCountG, lineG = $lineG (ops file [$lineCountO], lineO = $lineO\n");
        }
      }

      if ($verbosePrintPaths) {
        print "Ops line [$lineCountO]:: $lineO :: Golden paths <path> <slack>\n";
        foreach $path (keys %pathsG) {
          local @pathInfo = split(' ', $pathsG{$path});
          local $pathGSlack = $pathInfo[0];
          local $pathGNumber = $pathInfo[1];
          printf "path [%4d] = $path, slack = $pathGSlack\n", $pathGNumber;
        }
      }

      # populates contestant paths
      $pathKey .= " $lineCountC";

      %pathsC = ();

      while ($lineC = <FILEINC>) {
        chomp($lineC);
        $lineCountC++;
#        print "parse_worst_paths_contestant: lineCountC = $lineCountC, lineC = $lineC\n";

        if ($lineC =~ /report_worst_paths ([0-9]+)/) {
          $numPaths = $1;
          # no contestant paths
          if ($numPaths == 0) {
            last;
          }
        }
        # key is <pathType> <mode> <path: node transition node transition...>
        elsif ($lineC =~ /Path ([0-9]+): ([^\s]+) ([0-9e\-\.\+]+) ([0-9]+) ([EL])/) {
          $pathNumber = $1;
          $pathType   = $2;
          $pathSlack  = $3;
          $pathLength = $4;
          $pathMode   = $5;
          $path       = "$pathType $pathMode";
#          print "Processing Path $pathNumber of $numPaths, with type = $pathType, length = $pathLength, slack = $pathSlack, mode = $pathMode\n";
          $pathLengthCounter = 0;
        }
        elsif ($lineC =~ /([^\s]+) ([RF])/) {
          local $pathNode       = $1;
          local $pathTransition = $2;
          $pathLengthCounter++;
#          print "Current path node at $pathNode with transition $pathTransition\n";
          $path .= " $pathNode $pathTransition";
          if ($pathLengthCounter == $pathLength) {
            $pathsC{$path} = "$pathSlack $pathNumber";
#            print "Adding path number $pathNumber with nodes $path and slack $pathSlack\n";
            if ($pathNumber == $numPaths) {
              last;
            }
          }
        }
        else {
          die ("ERROR: unknown construct in contestant report_worst_paths, lineCountC = $lineCountC, lineC = $lineC (ops file [$lineCountO], lineO = $lineO)\n");
        }
      }

      if ($verbosePrintPaths) {
        print "Ops line [$lineCountO]:: $lineO :: Contestant paths <path> <slack>\n";
        foreach $path (keys %pathsC) {
          local @pathCInfo = split(' ', $pathsC{$path});
          local $pathCSlack = $pathCInfo[0];
          local $pathCNumber = $pathCInfo[1];
          printf "path [%4d] = $path, slack = $pathCSlack\n", $pathCNumber;
        }
      }

      # compare worst paths
      $pathScores{$pathKey} = &report_path_accuracy($lineCountO, $lineO);
    }
  }
  
  close (FILEINO);
  close (FILEING);
  close (FILEINC);

}


sub print_usage
{
  print "### Usage ###\n";
  print "REQUIRED inputs:\n";
  print "(1) <.ops file>              : the operations file used to generate goldens\n";
  print "(2) <golden output file>     : everything in this file is required to be other\n";
  print "(3) <contestant output file> : to be compared against golden\n";
  print "OPTIONAL inputs (must be after required arguments):\n";
  print "  -print_all_values               : prints all parsed at, rat, slack values\n";
  print "                                  : (default is OFF)\n";
  print "  -print_all_paths                : prints all parsed paths\n";
  print "                                  : (default is OFF)\n";
  print "  -print_detailed_value_accuracy  : reports all mismatched rat, at, slack values\n";
  print "                                  : (default is OFF)\n";
  print "  -print_detailed_path_accuracy   : reports all missing paths and paths with mismatched slacks\n";
  print "                                  : (default is OFF)\n";
}

## ----------------------- start main ------------------------- ##


local $argc = scalar(@ARGV);
unless ($argc > 1) {
  &print_usage();
  exit;
}

local $opsFile        = $ARGV[0];
local $goldenFile     = $ARGV[1];
local $contestantFile = $ARGV[2];

for (my $i = 3; $i < $argc; $i++) {
  if ($ARGV[$i] eq "-print_all_values") {
    $verbosePrintValues = 1;
  }
  elsif ($ARGV[$i] eq "-print_all_paths") {
    $verbosePrintPaths = 1;
  }
  elsif ($ARGV[$i] eq "-print_detailed_value_accuracy") {
    $printVerboseValueAccuracy = 1;
  }
  elsif ($ARGV[$i] eq "-print_detailed_path_accuracy") {
    $printVerbosePathAccuracy = 1;
  }
  else {
    printf "unknown command input %s\n", $ARGV[$i];
    &print_usage();
    exit;
  }
}

print "# ----------------------------------------------------------------------------------\n";
print "# output file (golden vs. contestant) comparer\n";
print "# ----------------------------------------------------------------------------------\n";
print "# Parsing files: (.ops)       $opsFile\n";
print "#                (Golden)     $goldenFile\n";
print "#                (Contestant) $contestantFile\n";
print "# Parameter settings:\n";
print "#  -print_all_values              : ";
if ($verbosePrintValues) {
  print "ON";
}
else {
  print "OFF";
}
print "\n";
print "#  -print_all_paths               : ";
if ($verbosePrintPaths) {
  print "ON";
}
else {
  print "OFF";
}
print "\n";
print "#  -print_detailed_value_accuracy : ";
if ($printVerboseValueAccuracy) {
  print "ON";
}
else {
  print "OFF";
}
print "\n";
print "#  -print_detailed_path_accuracy  : ";
if ($printVerbosePathAccuracy) {
  print "ON";
}
else {
  print "OFF";
}
print "\n";
print "# ==================================================================================\n";


&parse_ops_and_output_files($opsFile, $goldenFile, $contestantFile);

local $valueAverageScore = &report_values_accuracy();
local $pathAverageScore = &report_paths_accuracy();

print  "======================================================================\n";
printf "FINAL ACCURACY: Average of path and value is %3.5f\n", 
        ($valueAverageScore+$pathAverageScore)/2.*100.;
#printf "AVERAGE ACCURACY OF VALUE = %3.5f, AVERAGE ACCURACY OF PATH = %3.5f, FINAL ACCURACY = %3.5f\n", 
#        $valueAverageScore*100., $pathAverageScore*100., ($valueAverageScore+$pathAverageScore)/2.*100.;
print  "======================================================================\n";

