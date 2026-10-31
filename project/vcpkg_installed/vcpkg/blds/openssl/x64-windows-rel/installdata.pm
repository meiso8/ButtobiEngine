package OpenSSL::safe::installdata;

use strict;
use warnings;
use Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(
    @PREFIX
    @libdir
    @BINDIR @BINDIR_REL_PREFIX
    @LIBDIR @LIBDIR_REL_PREFIX
    @INCLUDEDIR @INCLUDEDIR_REL_PREFIX
    @APPLINKDIR @APPLINKDIR_REL_PREFIX
    @ENGINESDIR @ENGINESDIR_REL_LIBDIR
    @MODULESDIR @MODULESDIR_REL_LIBDIR
    @PKGCONFIGDIR @PKGCONFIGDIR_REL_LIBDIR
    @CMAKECONFIGDIR @CMAKECONFIGDIR_REL_LIBDIR
    $COMMENT $VERSION @LDLIBS
);

our $COMMENT                    = '';
our @PREFIX                     = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL' );
our @libdir                     = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL\lib' );
our @BINDIR                     = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL\bin' );
our @BINDIR_REL_PREFIX          = ( 'bin' );
our @LIBDIR                     = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL\lib' );
our @LIBDIR_REL_PREFIX          = ( 'lib' );
our @INCLUDEDIR                 = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL\include' );
our @INCLUDEDIR_REL_PREFIX      = ( 'include' );
our @APPLINKDIR                 = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL\include\openssl' );
our @APPLINKDIR_REL_PREFIX      = ( 'include/openssl' );
our @ENGINESDIR                 = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL\lib\engines-3' );
our @ENGINESDIR_REL_LIBDIR      = ( 'engines-3' );
our @MODULESDIR                 = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL\bin' );
our @MODULESDIR_REL_LIBDIR      = ( '..\bin' );
our @PKGCONFIGDIR               = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL\lib' );
our @PKGCONFIGDIR_REL_LIBDIR    = ( '' );
our @CMAKECONFIGDIR             = ( 'C:\Users\k024g\Lessons\GE3\project\vcpkg_installed\vcpkg\pkgs\openssl_x64-windows\Program Files\OpenSSL\share\openssl' );
our @CMAKECONFIGDIR_REL_LIBDIR  = ( '..\share\openssl' );
our $VERSION                    = '3.6.3';
our @LDLIBS                     =
    # Unix and Windows use space separation, VMS uses comma separation
    $^O eq 'VMS'
    ? split(/ *, */, 'ws2_32.lib gdi32.lib advapi32.lib crypt32.lib user32.lib ')
    : split(/ +/, 'ws2_32.lib gdi32.lib advapi32.lib crypt32.lib user32.lib ');

1;
