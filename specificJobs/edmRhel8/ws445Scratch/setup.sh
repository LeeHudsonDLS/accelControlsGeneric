#!/bin/sh
# Setup env for EDM testing

export EPICS_EXTENSIONS=/scratch/extensions
export EPICS_HOST_ARCH=linux-x86_64
cd ${EPICS_EXTENSIONS}/lib/${EPICS_HOST_ARCH}
export EDMOBJECTS=$PWD
export EDMPVOBJECTS=$PWD
export EDM=${EPICS_EXTENSIONS}/bin/${EPICS_HOST_ARCH}/edm

export EDMOBJECTS=$EPICS_EXTENSIONS/etc
export EDMPVOBJECTS=$EPICS_EXTENSIONS/etc
export EDMFILES=$EPICS_EXTENSIONS/src/edm/edmMain
export EDMHELPFILES=$EPICS_EXTENSIONS/src/edm/helpFiles
export EDMLIBS=$EPICS_EXTENSIONS/lib/$EPICS_HOST_ARCH
export EDMFONTFILE=/scratch/fonts.list

export PATH="${EPICS_EXTENSIONS}/bin/${EPICS_HOST_ARCH}:${PATH}"
export PATH="/scratch/extensions/fonts/arial:${PATH}"
export PATH="/scratch/extensions/fonts/courier:${PATH}"

xset +fp /scratch/extensions/fonts/arial
xset +fp /scratch/extensions/fonts/courier
xset fp rehash
cd /scratch
