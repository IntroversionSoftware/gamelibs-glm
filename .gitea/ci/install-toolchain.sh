#!/bin/sh
# Install the CI build toolchain on a Debian/Ubuntu runner image.
# Usage: install-toolchain.sh [--clang] [--libcxx] <extra apt packages...>
# Installs cmake, ninja and the extra packages; with --clang, also clang-$CLANG_VERSION, from the
# distribution if it has that version, otherwise from apt.llvm.org; with --libcxx, also that
# version's libc++ (for -stdlib=libc++), from the same place.
set -eu

CLANG=0
LIBCXX=0
while [ $# -gt 0 ]; do
	case "$1" in
		--clang) CLANG=1 ;;
		--libcxx) LIBCXX=1 ;;
		*) break ;;
	esac
	shift
done

SUDO=
[ "$(id -u)" = 0 ] || SUDO=sudo
export DEBIAN_FRONTEND=noninteractive

$SUDO apt-get update -qq
$SUDO apt-get install -y -qq --no-install-recommends ca-certificates gnupg wget cmake ninja-build "$@"
cmake --version | head -1

if [ "$CLANG" = 1 ]; then
	# Older clang releases (Ubuntu 24.04's 18 and 20) miscompile some GLM code on AArch64.
	if ! apt-cache show "clang-$CLANG_VERSION" > /dev/null 2>&1; then
		. /etc/os-release
		# Plain HTTP so a caching proxy in front of apt.llvm.org can serve it (as the apt source
		# itself does); the proxy fetches from apt.llvm.org over HTTPS.
		wget -qO- http://apt.llvm.org/llvm-snapshot.gpg.key | gpg --dearmor | \
			$SUDO tee /usr/share/keyrings/apt.llvm.org.gpg > /dev/null
		echo "deb [signed-by=/usr/share/keyrings/apt.llvm.org.gpg] http://apt.llvm.org/$VERSION_CODENAME/ llvm-toolchain-$VERSION_CODENAME-$CLANG_VERSION main" | \
			$SUDO tee /etc/apt/sources.list.d/apt.llvm.org.list > /dev/null
		$SUDO apt-get update -qq
	fi
	PACKAGES="clang-$CLANG_VERSION"
	if [ "$LIBCXX" = 1 ]; then
		PACKAGES="$PACKAGES libc++-$CLANG_VERSION-dev libc++abi-$CLANG_VERSION-dev"
	fi
	$SUDO apt-get install -y -qq --no-install-recommends $PACKAGES
	"clang++-$CLANG_VERSION" --version | head -1
fi
