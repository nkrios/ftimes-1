#!/usr/bin/perl -w
######################################################################
#
# $Id: hashdig-bind.pl,v 1.15 2004/04/26 03:13:52 mavrik Exp $
#
######################################################################
#
# Copyright 2003-2004 The FTimes Project, All Rights Reserved.
#
######################################################################
#
# Purpose: Bind resolved hashes to filenames.
#
######################################################################

use strict;
use File::Basename;
use FileHandle;
use Getopt::Std;

######################################################################
#
# Main Routine
#
######################################################################

  ####################################################################
  #
  # Punch in and go to work.
  #
  ####################################################################

  my ($sProgram, %hProperties);

  $sProgram = $hProperties{'program'} = basename(__FILE__);

  ####################################################################
  #
  # Get Options.
  #
  ####################################################################

  my (%hOptions);

  if (!getopts('f:qrt:', \%hOptions))
  {
    Usage($sProgram);
  }

  ####################################################################
  #
  # A filename is required, and can be '-' or a regular file.
  #
  ####################################################################

  my ($sFileHandle, $sFilename);

  if (!exists($hOptions{'f'}) || !defined($hOptions{'f'}) || length($hOptions{'f'}) < 1)
  {
    Usage($sProgram);
  }
  $sFilename = $hOptions{'f'};

  if ($sFilename eq '-')
  {
    $sFileHandle = \*STDIN;
  }
  else
  {
    if (!-f $sFilename)
    {
      print STDERR "$sProgram: File='$sFilename' Error='File must exist and be regular.'\n";
      exit(2);
    }
    if (!open(FH, "<$sFilename"))
    {
      print STDERR "$sProgram: File='$sFilename' Error='$!'\n";
      exit(2);
    }
    $sFileHandle = \*FH;
  }

  ####################################################################
  #
  # The BeQuiet flag, '-q', is optional. Default value is 0.
  #
  ####################################################################

  $hProperties{'BeQuiet'} = (exists($hOptions{'q'})) ? 1 : 0;

  ####################################################################
  #
  # The ReverseFormat flag, '-r', is optional. Default value is 0.
  #
  ####################################################################

  my ($sCIndex, $sHIndex, $sRecordRegex, $sReverseFormat);

  $sReverseFormat = (exists($hOptions{'r'})) ? 1 : 0;

  if ($sReverseFormat)
  {
    $sRecordRegex = qq(^([KU])\\|([0-9a-fA-F]{32})\$);
    $sCIndex = 0;
    $sHIndex = 1;
  }
  else
  {
    $sRecordRegex = qq(^([0-9a-fA-F]{32})\\|([KU])\$);
    $sCIndex = 1;
    $sHIndex = 0;
  }

  ####################################################################
  #
  # The FileType flag, '-t', is required.
  #
  ####################################################################

  my ($sBindFile, $sFileType);

  $sFileType = (exists($hOptions{'t'})) ? $hOptions{'t'} : undef;

  if (!defined($sFileType))
  {
    Usage($sProgram);
  }

  if ($sFileType =~ /^FTIMES$/i)
  {
    $sBindFile = \&BindFTimesFile;
  }
  elsif ($sFileType =~ /^(KG|KNOWNGOODS)$/i)
  {
    $sBindFile = \&BindKnownGoodsFile;
  }
  elsif ($sFileType =~ /^(MD5|OPENSSL)$/i)
  {
    $sBindFile = \&BindMD5File;
  }
  elsif ($sFileType =~ /^(MD5SUM|MD5DEEP)$/i)
  {
    $sBindFile = \&BindMD5SumFile;
  }
  else
  {
    print STDERR "$sProgram: FileType='$sFileType' Error='Invalid file type.'\n";
    exit(2);
  }

  ####################################################################
  #
  # If there isn't at least one argument left, it's an error.
  #
  ####################################################################

  if (scalar(@ARGV) < 1)
  {
    Usage($sProgram);
  }

  ####################################################################
  #
  # Read hashes and create known and unknown hash lists.
  #
  ####################################################################

  my (%hHashKList, %hHashUList);

  while (my $sRecord = <$sFileHandle>)
  {
    $sRecord =~ s/[\r\n]+$//;
    if (my @aFields = $sRecord =~ /$sRecordRegex/o)
    {
      $aFields[$sHIndex] = lc($aFields[$sHIndex]);
      if ($aFields[$sCIndex] eq "K")
      {
        $hHashKList{$aFields[$sHIndex]}++;
      }
      else
      {
        $hHashUList{$aFields[$sHIndex]}++;
      }
    }
    else
    {
      print STDERR "$sProgram: File='$sFilename' Record='$sRecord' Error='Record did not parse properly.'\n";
      exit(2);
    }
  }
  close($sFileHandle);

  ####################################################################
  #
  # Iterate over input files.
  #
  ####################################################################

  foreach my $sInputFile (@ARGV)
  {
    &$sBindFile($sInputFile, \%hProperties);
  }

  ####################################################################
  #
  # Cleanup and go home.
  #
  ####################################################################

  1;


######################################################################
#
# BindFTimesFile
#
######################################################################

sub BindFTimesFile
{
  my ($sFilename, $phProperties) = @_;

  ####################################################################
  #
  # Open input file.
  #
  ####################################################################

  if (!open(FH, "<$sFilename"))
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Error='$!'\n";
    }
    return undef;
  }

  ####################################################################
  #
  # Process header.
  #
  ####################################################################

  my (@aFields, $sHashIndex, $sHeader, $sModeIndex, $sNameIndex);

  $sHeader = <FH>;
  if (defined($sHeader))
  {
    $sHeader =~ s/[\r\n]+$//;
    @aFields = split(/\|/, $sHeader, -1);
    for (my $sIndex = 0; $sIndex < scalar(@aFields); $sIndex++)
    {
      if ($aFields[$sIndex] =~ /^mode$/o)
      {
        $sModeIndex = $sIndex;
      }
      elsif ($aFields[$sIndex] =~ /^md5$/o)
      {
        $sHashIndex = $sIndex;
      }
      elsif ($aFields[$sIndex] =~ /^name$/o)
      {
        $sNameIndex = $sIndex;
      }
    }

    if (!defined($sHashIndex) || !defined($sNameIndex))
    {
      if (!$$phProperties{'BeQuiet'})
      {
        print STDERR "$$phProperties{'program'}: File='$sFilename' Header='$sHeader' Error='Header did not parse properly.'\n";
      }
      close(FH);
      return undef;
    }
  }
  else
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Header='' Error='Header did not parse properly.'\n";
    }
    close(FH);
    return undef;
  }

  ####################################################################
  #
  # Open output files.
  #
  ####################################################################

  my (@aHandles, %hHandleList);

  @aHandles = ("a", "d", "i", "k", "l", "s", "u");

  if (!defined(OpenFileHandles($sFilename, \@aHandles, \%hHandleList)))
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Error='Unable to create one or more output files.'\n";
    }
    close(FH);
    return undef;
  }

  ####################################################################
  #
  # Process records.
  #
  ####################################################################

  my ($sCategory, $sCategoryHandle, $sCombinedHandle, $sHash);

  $sCombinedHandle = $hHandleList{'a'};

  while (my $sRecord = <FH>)
  {
    $sRecord =~ s/[\r\n]+$//;
    @aFields = split(/\|/, $sRecord, -1);
    if (defined($aFields[$sHashIndex]) && defined($aFields[$sNameIndex]))
    {
      $sHash = lc($aFields[$sHashIndex]);

      if ($hHashUList{$sHash})
      {
        $sCategory = "U";
        $sCategoryHandle = $hHandleList{'u'};
      }
      elsif ($hHashKList{$sHash})
      {
        $sCategory = "K";
        $sCategoryHandle = $hHandleList{'k'};
      }
      elsif ($sHash =~ /^directory$/o)
      {
        $sCategory = "D";
        $sCategoryHandle = $hHandleList{'d'};
        $sHash = uc($sHash);
      }
      elsif ($sHash =~ /^symlink$/o)
      {
        $sCategory = "L";
        $sCategoryHandle = $hHandleList{'l'};
        $sHash = uc($sHash);
      }
      elsif (defined($sModeIndex) && $aFields[$sModeIndex] =~ /^12[0-7]{4}$/o)
      {
        $sCategory = "L";
        $sCategoryHandle = $hHandleList{'l'};
      }
      elsif ($sHash =~ /^special$/o)
      {
        $sCategory = "S";
        $sCategoryHandle = $hHandleList{'s'};
        $sHash = uc($sHash);
      }
      else
      {
        $sCategory = "I";
        $sCategoryHandle = $hHandleList{'i'};
      }

      $aFields[$sNameIndex] =~ s/^"(.*)"$/$1/; # Remove double quotes around the name.

      print $sCombinedHandle "$sCategory|$sHash|$aFields[$sNameIndex]\n";
      print $sCategoryHandle "$sCategory|$sHash|$aFields[$sNameIndex]\n";
    }
    else
    {
      if (!$$phProperties{'BeQuiet'})
      {
        print STDERR "$$phProperties{'program'}: Record='$sRecord' Error='Record did not parse properly.'\n";
      }
    }
  }

  ####################################################################
  #
  # Cleanup.
  #
  ####################################################################

  foreach my $sHandle (keys(%hHandleList))
  {
    close($hHandleList{$sHandle});
  }
  close(FH);

  return 1;
}


######################################################################
#
# BindKnownGoodsFile
#
######################################################################

sub BindKnownGoodsFile
{
  my ($sFilename, $phProperties) = @_;

  ####################################################################
  #
  # Open input file.
  #
  ####################################################################

  if (!open(FH, "<$sFilename"))
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Error='$!'\n";
    }
    return undef;
  }

  ####################################################################
  #
  # Process header.
  #
  ####################################################################

  my ($sFieldCount, @aFields, $sHashIndex, $sHeader, $sNameIndex);

  $sHeader = "ID,FILENAME,MD5,SHA-1,SIZE,TYPE,PLATFORM,PACKAGE";
  if (defined($sHeader))
  {
    $sHeader =~ s/[\r\n]+$//;
    @aFields = split(/,/, $sHeader, -1);
    for (my $sIndex = 0; $sIndex < scalar(@aFields); $sIndex++)
    {
      if ($aFields[$sIndex] =~ /^FILENAME$/o)
      {
        $sNameIndex = $sIndex;
      }
      elsif ($aFields[$sIndex] =~ /^MD5$/o)
      {
        $sHashIndex = $sIndex;
      }
    }
    $sFieldCount = scalar(@aFields);

    if (!defined($sHashIndex) || !defined($sNameIndex))
    {
      if (!$$phProperties{'BeQuiet'})
      {
        print STDERR "$$phProperties{'program'}: File='$sFilename' Header='$sHeader' Error='Header did not parse properly.'\n";
      }
      close(FH);
      return undef;
    }
  }
  else
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Header='' Error='Header did not parse properly.'\n";
    }
    close(FH);
    return undef;
  }

  ####################################################################
  #
  # Open output files.
  #
  ####################################################################

  my (@aHandles, %hHandleList);

  @aHandles = ("a", "i", "k", "u");

  if (!defined(OpenFileHandles($sFilename, \@aHandles, \%hHandleList)))
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Error='Unable to create one or more output files.'\n";
    }
    close(FH);
    return undef;
  }

  ####################################################################
  #
  # Process records.
  #
  ####################################################################

  my ($sCategory, $sCategoryHandle, $sCombinedHandle, $sCount, $sHash, $sName);

  $sCombinedHandle = $hHandleList{'a'};

  while (my $sRecord = <FH>)
  {
    $sRecord =~ s/[\r\n]+$//;
    @aFields = split(/,/, $sRecord, -1);
    $sCount = scalar(@aFields);
    if (defined($aFields[$sHashIndex]) && defined($aFields[$sNameIndex]) && $sCount >= $sFieldCount)
    {
      if ($sCount > $sFieldCount)
      {
        my $sLIndex = $sNameIndex;
        my $sHIndex = $sCount - $sFieldCount + $sHashIndex - 1;
        $sName = join(',', @aFields[$sLIndex..$sHIndex]);
        $sHash = lc($aFields[$sHIndex + 1]);
      }
      else
      {
        $sName = $aFields[$sNameIndex];
        $sHash = lc($aFields[$sHashIndex]);
      }

      if ($hHashKList{$sHash})
      {
        $sCategory = "K";
        $sCategoryHandle = $hHandleList{'k'};
      }
      elsif ($hHashUList{$sHash})
      {
        $sCategory = "U";
        $sCategoryHandle = $hHandleList{'u'};
      }
      else
      {
        $sCategory = "I";
        $sCategoryHandle = $hHandleList{'i'};
      }

      print $sCombinedHandle "$sCategory|$sHash|$sName\n";
      print $sCategoryHandle "$sCategory|$sHash|$sName\n";
    }
    else
    {
      if (!$$phProperties{'BeQuiet'})
      {
        print STDERR "$$phProperties{'program'}: Record='$sRecord' Error='Record did not parse properly.'\n";
      }
    }
  }

  ####################################################################
  #
  # Cleanup.
  #
  ####################################################################

  foreach my $sHandle (keys(%hHandleList))
  {
    close($hHandleList{$sHandle});
  }
  close(FH);

  return 1;
}


######################################################################
#
# BindMD5File
#
######################################################################

sub BindMD5File
{
  my ($sFilename, $phProperties) = @_;

  ####################################################################
  #
  # Open input file.
  #
  ####################################################################

  if (!open(FH, "<$sFilename"))
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Error='$!'\n";
    }
    return undef;
  }

  ####################################################################
  #
  # Process header.
  #
  ####################################################################

  # This format has no header.

  ####################################################################
  #
  # Open output files.
  #
  ####################################################################

  my (@aHandles, %hHandleList);

  @aHandles = ("a", "i", "k", "u");

  if (!defined(OpenFileHandles($sFilename, \@aHandles, \%hHandleList)))
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Error='Unable to create one or more output files.'\n";
    }
    close(FH);
    return undef;
  }

  ####################################################################
  #
  # Process records.
  #
  ####################################################################

  my ($sCategory, $sCategoryHandle, $sCombinedHandle, $sHash, $sName);

  $sCombinedHandle = $hHandleList{'a'};

  while (my $sRecord = <FH>)
  {
    $sRecord =~ s/[\r\n]+$//;
    if (($sName, $sHash) = $sRecord =~ /^MD5\s*\((.*)\)\s*=\s+([0-9a-fA-F]{32})$/o)
    {
      $sHash = lc($sHash);

      if ($hHashKList{$sHash})
      {
        $sCategory = "K";
        $sCategoryHandle = $hHandleList{'k'};
      }
      elsif ($hHashUList{$sHash})
      {
        $sCategory = "U";
        $sCategoryHandle = $hHandleList{'u'};
      }
      else
      {
        $sCategory = "I";
        $sCategoryHandle = $hHandleList{'i'};
      }

      print $sCombinedHandle "$sCategory|$sHash|$sName\n";
      print $sCategoryHandle "$sCategory|$sHash|$sName\n";
    }
    else
    {
      if (!$$phProperties{'BeQuiet'})
      {
        print STDERR "$$phProperties{'program'}: Record='$sRecord' Error='Record did not parse properly.'\n";
      }
    }
  }

  ####################################################################
  #
  # Cleanup.
  #
  ####################################################################

  foreach my $sHandle (keys(%hHandleList))
  {
    close($hHandleList{$sHandle});
  }
  close(FH);

  return 1;
}


######################################################################
#
# BindMD5SumFile
#
######################################################################

sub BindMD5SumFile
{
  my ($sFilename, $phProperties) = @_;

  ####################################################################
  #
  # Open input file.
  #
  ####################################################################

  if (!open(FH, "<$sFilename"))
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Error='$!'\n";
    }
    return undef;
  }

  ####################################################################
  #
  # Process header.
  #
  ####################################################################

  # This format has no header.

  ####################################################################
  #
  # Open output files.
  #
  ####################################################################

  my (@aHandles, %hHandleList);

  @aHandles = ("a", "i", "k", "u");

  if (!defined(OpenFileHandles($sFilename, \@aHandles, \%hHandleList)))
  {
    if (!$$phProperties{'BeQuiet'})
    {
      print STDERR "$$phProperties{'program'}: File='$sFilename' Error='Unable to create one or more output files.'\n";
    }
    close(FH);
    return undef;
  }

  ####################################################################
  #
  # Process records.
  #
  ####################################################################

  my ($sCategory, $sCategoryHandle, $sCombinedHandle, $sHash, $sName);

  $sCombinedHandle = $hHandleList{'a'};

  while (my $sRecord = <FH>)
  {
    $sRecord =~ s/[\r\n]+$//;
    if (($sHash, $sName) = $sRecord =~ /^([0-9a-fA-F]{32})\s+(.*)\s*$/o)
    {
      $sHash = lc($sHash);

      if ($hHashKList{$sHash})
      {
        $sCategory = "K";
        $sCategoryHandle = $hHandleList{'k'};
      }
      elsif ($hHashUList{$sHash})
      {
        $sCategory = "U";
        $sCategoryHandle = $hHandleList{'u'};
      }
      else
      {
        $sCategory = "I";
        $sCategoryHandle = $hHandleList{'i'};
      }

      print $sCombinedHandle "$sCategory|$sHash|$sName\n";
      print $sCategoryHandle "$sCategory|$sHash|$sName\n";
    }
    else
    {
      if (!$$phProperties{'BeQuiet'})
      {
        print STDERR "$$phProperties{'program'}: Record='$sRecord' Error='Record did not parse properly.'\n";
      }
    }
  }

  ####################################################################
  #
  # Cleanup.
  #
  ####################################################################

  foreach my $sHandle (keys(%hHandleList))
  {
    close($hHandleList{$sHandle});
  }
  close(FH);

  return 1;
}


######################################################################
#
# OpenFileHandles
#
######################################################################

sub OpenFileHandles
{
  my ($sInputFile, $paHandles, $phHandleList) = @_;

  my ($sFailures, $sOutBase);

  my ($sName, $sPath, $sSuffix) = fileparse($sInputFile);

  $sOutBase = $sName . ".bound";

  $sFailures = 0;

  foreach my $sExtension (@$paHandles)
  {
    $$phHandleList{$sExtension} = new FileHandle(">$sOutBase.$sExtension");
    if (!defined($$phHandleList{$sExtension}))
    {
      $sFailures++;
    }
  }

  if ($sFailures)
  {
    foreach my $sExtension (@$paHandles)
    {
      if (exists($$phHandleList{$sExtension}) && defined($$phHandleList{$sExtension}))
      {
        close($$phHandleList{$sExtension});
      }
      unlink("$sOutBase.$sExtension");
    }
    return undef;
  }

  return 1;
}


######################################################################
#
# Usage
#
######################################################################

sub Usage
{
  my ($sProgram) = @_;
  print STDERR "\n";
  print STDERR "Usage: $sProgram [-q] [-r] -t type -f {hashdig-file|-} file [file ...]\n";
  print STDERR "\n";
  exit(1);
}


=pod

=head1 NAME

hashdig-bind.pl - Bind resolved hashes to filenames

=head1 SYNOPSIS

B<hashdig-bind.pl> B<[-q]> B<[-r]> B<-t type> B<-f {hashdig-file|-}> B<file [file ...]>

=head1 DESCRIPTION

This utility binds resolved hashes to filenames. The source of
resolved hashes is a HashDig file or stdin, and the source of
filenames is one or more subject files. Depending on the type of
subject files (see B<-t> option), one or more of the following
output files will be created in the current working directory:
(a)ll, (d)irectory, (i)ndeterminate, (k)nown, symbolic (l)ink,
(s)pecial, and (u)nknown. These files will have the following
format:

    <filename>.bound.{a|d|i|k|l|s|u}

The 'all' file is the sum of the other output files.

=head1 OPTIONS

=over 4

=item B<-f {hashdig-file|-}>

Specifies the name of a HashDig file to use as the source of hashes.
A value of '-' will cause the program to read from stdin. HashDig
files have the following format:

    hash|category

=item B<-q>

Don't report errors (i.e. be quiet) while processing files.

=item B<-r>

Accept HashDig records in reverse format (i.e. category|hash).

=item B<-t type>

Specifies the type of subject files that are to be processed. All
files processed in a given invocation must be of the same type.
Currently, the following types are supported: FTIMES, KG|KNOWNGOODS,
MD5, MD5DEEP, MD5SUM, and OPENSSL. The value for this option is not
case sensitive.

=back

=head1 CAVEATS

This utility attempts to load all hash/category information into a
pair associative arrays. When all available memory has been exhausted,
Perl will probably force the script to abort. In extreme cases,
this can produce a core file.

=head1 AUTHOR

Klayton Monroe

=head1 SEE ALSO

ftimes(1), hashdig-dump.pl, hashdig-harvest.pl, hashdig-harvest-sunsolve.pl, md5(1), md5sum(1), md5deep, openssl(1)

=head1 LICENSE

All HashDig documentation and code is distributed under same terms
and conditions as FTimes.

=cut
