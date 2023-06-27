#!/usr/bin/perl
#
# Copyright (c) 2021 Alibaba Group.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#    THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
#    CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
#    LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
#    FOR A PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
#
#    See the Apache Version 2.0 License for specific language governing
#    permissions and limitations under the License.
#
# @file    spec.pm
#
# @brief   This module generates OTAI-spec document.
#

package spec;

use strict;
use warnings;
use diagnostics;

use Getopt::Std;
use Data::Dumper;
use utils;
use xmlutils;
use style;
use serialize;
use cap;
use Excel::Writer::XLSX;

require Exporter;

sub GenLaiSpecFile
{
    my $spec_file = 'OTAI-' . $main::OTAI_VER . '-spec.xlsx';
    my $workbook = Excel::Writer::XLSX->new($spec_file);
    my $worksheet = $workbook->add_worksheet('OTAI');
    $worksheet->write( "A1", "Object" );
    $worksheet->write( "B1", "Atrribute" );
    $worksheet->write( "C1", "Type" );
    $worksheet->write( "D1", "R/W" );
    $worksheet->write( "E1", "Precision" );
    $worksheet->write( "F1", "Unit" );
    $worksheet->write( "G1", "PM-Type" );

    my $worksheet2 = $workbook->add_worksheet('Alarm');
    $worksheet2->write(0, 0, "OTAI Alarm Enum");

    my $x_pos = 1;
    for my $key (sort keys %main::OTAI_ENUMS)
    {
        if ($key =~ /^(otai_(\w+)_attr_t)$/)
        {
            my $typedef = $1;
            next if $typedef =~ /null/;
            $worksheet->write($x_pos, 0, "$2");
            my $enum = $main::OTAI_ENUMS{$typedef};
            my @values = @{ $enum->{values} };
            for my $attr (@values)
            {
               my %meta = %{ $main::METADATA{$typedef}{$attr} };
               my $flags = $meta{flags};
               my @flags = @{ $flags };
               $flags = join("|",@flags);
               my $rw = "R";
               if ($flags =~ /CREATE_ONLY/)
               {
                   $rw = "CREATE_ONLY";
               }
               elsif ($flags =~ /CREATE_AND_SET/)
               {
                   $rw = "RW";
               }
               elsif ($flags =~ /SET_ONLY/)
               {
                   $rw = "W";
               }
               $worksheet->write($x_pos, 1, "$attr");
               $worksheet->write($x_pos, 2, "$meta{type}");
               $worksheet->write($x_pos, 3, "$rw");
               $x_pos = $x_pos + 1;
            }
        }
        elsif ($key =~ /^(otai_(\w+)_stat_t)$/)
        {
            my $typedef = $1;
            my $enum = $main::OTAI_ENUMS{$typedef};
            my @values = @{ $enum->{values} };
            for my $stat (@values)
            {
               next if $stat =~ /NOT_USE$/;
               my %meta = %{ $main::METADATA{$typedef}{$stat} };
               my $rw = "R";
               $worksheet->write($x_pos, 1, "$stat");
               $worksheet->write($x_pos, 2, "$meta{type}");
               $worksheet->write($x_pos, 3, "$rw");
               if (defined $meta{precision})
               {
                   $worksheet->write($x_pos, 4, "$meta{precision}");
               }
               if (defined $meta{unit})
               {
                   $worksheet->write($x_pos, 5, "$meta{unit}");
               }
               if (defined $meta{iscounter})
               {
                   if ($meta{iscounter} =~ /false/)
                   {
                       $worksheet->write($x_pos, 6, "gauge");
                   }
                   else
                   {
                       $worksheet->write($x_pos, 6, "counter");
                   }
               }
               $x_pos = $x_pos + 1;
            }
        } elsif ($key =~ /^otai_alarm_type_t$/)
        {
            my $enum = $main::OTAI_ENUMS{$key};
            my @values = @{ $enum->{values} };
            my $alarm_x_pos = 1;
            for my $alarm_id (@values)
            {
               next if $alarm_id =~ /OTAI_ALARM_TYPE_MAX$/;
               $worksheet2->write($alarm_x_pos, 0, $alarm_id);
               $alarm_x_pos = $alarm_x_pos + 1;
            }
        }
    }

    $workbook->close;
}

BEGIN
{
    our @ISA    = qw(Exporter);
    our @EXPORT = qw/
    GenLaiSpecFile
    /;
}

1;

