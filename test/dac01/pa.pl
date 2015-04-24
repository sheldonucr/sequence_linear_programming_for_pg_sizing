#! /usr/local/bin/perl -w
my $usage = "Usage: $0 infile\n";

if(@ARGV ne 1) {
  print $usage;
  exit;
}

while(<>) {

    @f = split;
    if(defined($f[0]) && ($f[0] eq "CONST")) {
	printf"$f[0] $f[1] $f[2] 3\n";
	next;
	} 
    else {
	print $_;
	next;
	}
}
