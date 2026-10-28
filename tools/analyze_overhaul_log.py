#!/usr/bin/env python3
"""Summarize game logs without mistaking missing diagnostics for failed mechanics.

Usage: python tools/analyze_overhaul_log.py "C:\path\cos_....log"
Python 3 standard library only. Also accepts older pre-OVDIAG logs.
"""
import argparse
import collections
import pathlib
import re

GROUPS = {
    "Ship acceleration / galley ram": ["NavalRamming INIT", "NavalRamming ACCEL_START", "NavalRamming ACCEL_50", "NavalRamming ACCEL_75", "NavalRamming FULL_SPEED", "NavalRamming HIT"],
    "Cavalry formation / motion": ["OVDIAG CAV_INIT", "OVDIAG CAV_STATE", "OVDIAG CAV_BLOCK reason=NO_FORMATION", "OVDIAG CAV_BOOST_START", "OVDIAG CAV_BOOST_MAX", "OVDIAG CAV_BOOST_END"],
    "Cavalry contact / ram": ["OVDIAG CAV_ENEMY", "OVDIAG CAV_CONTACT", "OVDIAG CAV_BLOCK reason=PIKES", "OVDIAG CAV_HIT_CHECK", "CavalryCharge HIT"],
    "Applied combat / reduced damage": ["OVDIAG DAMAGE_GLOBAL", "OVDIAG HP_APPLY", "OVDIAG RELOAD"],
    "Artillery / buildings configuration": ["OVDIAG CONFIG_SPAWN", "OVDIAG GUN_INIT", "OVDIAG MORTAR_MEMBER_DISABLED"],
    "AI artillery": ["OVDIAG AI_ORDER", "OVDIAG AI_ART_LIMIT"],
}
OUT_OF_SCOPE = [
    "Physical knockback through three rows",
    "Full pike-formation collision wall",
    "Two human crew required for cannon/howitzer",
    "Improved cannon/ship pathfinding and inertia on stop",
    "Depleting stone resources",
    "Fortress building with art/production",
    "Eight actual guns in an upgraded tower",
    "Once-per-minute soldier gold upkeep and mass mutiny",
    "Faster starvation deaths",
    "85% casualties automatic retreat",
]
def report(text):
    lines=text.splitlines()
    counts=collections.Counter()
    examples={}
    for line in lines:
        for group, markers in GROUPS.items():
            for marker in markers:
                if marker in line:
                    counts[marker] += 1
                    examples.setdefault(marker,line.strip())
    parts = [
        "Gameplay Overhaul log audit",
        "="*28,
        f"Total log lines: {len(lines)}",
        "Important: no log marker may mean not tested, a disabled feature, "
        "or no logging; it does NOT prove a malfunction.",
    ]
    for group, markers in GROUPS.items():
        parts.append("\n"+group)
        for marker in markers:
            count=counts[marker]
            status=f"OBSERVED {count}" if count else "NOT OBSERVED (unknown)"
            parts.append(f"  {marker}: {status}")
        shown=[marker for marker in markers if counts[marker]]
        if shown:
            parts.append("  Example: "+examples[shown[-1]][:240])
    cfg=re.findall(r"OVDIAG CONFIG sid=([a-z0-9_]+).*?hp=(\d+)",text)
    if cfg:
        parts.append("\nConfigured HP samples (verify against stock baselines):")
        for sid,hp in list(dict(cfg).items())[:35]:
            parts.append(f"  {sid}: {hp}")
    old_config_count=sum("OVDIAG CONFIG sid=" in line for line in lines)
    if old_config_count:
        parts.append("\nDIAGNOSTIC SPAM WARNING: "
                     +str(old_config_count)
                     +" old prototype CONFIG lines, not actual new units. "
                     +"Rebuild with the latest diagnostics generator to use CONFIG_SPAWN.")
    hp_events=[line for line in lines if "OVDIAG HP_APPLY" in line]
    charge_events=[line for line in hp_events if "requested=500000" in line]
    ship_events=[line for line in hp_events if "requested=8000" in line and "attacker=galley" in line]
    if hp_events:
        parts.append("\nVERIFIED HP CHANGES (not merely attack triggers):")
        parts.append("  All logged HP_APPLY events: "+str(len(hp_events)))
        parts.append("  Raw 500,000 cavalry charges with HP changes: "+str(len(charge_events)))
        parts.append("  Raw 8,000 galley rams with HP changes: "+str(len(ship_events)))
        for example in (charge_events[:1] + ship_events[:1]):
            parts.append("  Example: "+example.strip()[:280])
    parts.append("\nNOT IMPLEMENTED in phase 1 (no logs can validate these):")
    parts.extend("  - "+s for s in OUT_OF_SCOPE)
    parts.append("\nTip: on a small offline test map generate the diagnostic build "
                 "then upload the NEW game log. Full mode is too verbose for "
                 "large/long battles.")
    return "\n".join(parts)+"\n"

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument("log",type=pathlib.Path)
    ap.add_argument("-o","--output",type=pathlib.Path)
    args=ap.parse_args()
    try:
        data=args.log.read_bytes().decode("utf-8-sig")
    except UnicodeDecodeError:
        data=args.log.read_bytes().decode("cp1252",errors="replace")
    summary=report(data)
    if args.output:
        args.output.write_text(summary, encoding="utf-8")
        print(f"Saved {args.output}")
    else:
        print(summary)

if __name__=="__main__":
    main()
