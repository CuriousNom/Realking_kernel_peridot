_platform_map = {
    "autogvm": {
        "dtb_list": [
            # keep sorted
            {"name": "direwolf-vm-la.dtb"},
            {"name": "direwolf-vm-lv.dtb"},
            {"name": "direwolf-vm-lv-headless-mt.dtb"},
            {"name": "lemans-vm-la.dtb"},
            {"name": "lemans-vm-la-mt.dtb"},
            {"name": "lemans-vm-la1-mt.dtb"},
            {"name": "lemans-vm-lv.dtb"},
            {"name": "lemans-vm-lv-mt.dtb"},
            {"name": "sa8155-vm-la.dtb"},
            {"name": "sa8155-vm-la-mt.dtb"},
            {"name": "sa8195-vm-la.dtb"},
            {"name": "monaco-vm-la.dtb"},
            {"name": "monaco-vm-lv.dtb"},
            {"name": "sa6155p-vm-la.dtb"},
        ],
        "dtbo_list": [
            # keep sorted
            {"name": "direwolf-vm-la-overlay.dtbo"},
            {"name": "direwolf-vm-lv-overlay.dtbo"},
            {"name": "direwolf-vm-lv-headless-mt-overlay.dtbo"},
            {"name": "lemans-vm-la-overlay.dtbo"},
            {"name": "lemans-vm-la-mt-overlay.dtbo"},
            {"name": "lemans-vm-la1-mt-overlay.dtbo"},
            {"name": "lemans-vm-lv-overlay.dtbo"},
            {"name": "lemans-vm-lv-mt-overlay.dtbo"},
            {"name": "sa8155-vm-la-overlay.dtbo"},
            {"name": "sa8155-vm-la-mt-overlay.dtbo"},
            {"name": "sa8195-vm-la-overlay.dtbo"},
            {"name": "monaco-vm-la-overlay.dtbo"},
            {"name": "monaco-vm-lv-overlay.dtbo"},
            {"name": "sa6155p-vm-la-overlay.dtbo"},
        ],
    },
    "autoghgvm": {
        "dtb_list": [
            # keep sorted
            {"name": "lemans-gunyah-vm-cob.dtb"},
            {"name": "lemans-gunyah-vm-qam.dtb"},
            {"name": "lemans-gunyah-vm-vp.dtb"},
            {"name": "monaco-gunyah-vm-qam.dtb"},
        ],
        "dtbo_list": [
            # keep sorted
            {"name": "lemans-gunyah-vm-cob-overlay.dtbo"},
            {"name": "lemans-gunyah-vm-qam-overlay.dtbo"},
            {"name": "lemans-gunyah-vm-vp-overlay.dtbo"},
            {"name": "monaco-gunyah-vm-qam-overlay.dtbo"},
        ],
    },
    "gen3auto": {
        "dtb_list": [
            # keep sorted
            {"name": "sa8155.dtb"},
            {"name": "sa8155p.dtb"},
            {"name": "sa8155-v2.dtb"},
            {"name": "sa8155p-v2.dtb"},
            {"name": "sa8195p.dtb"},
        ],
        "dtbo_list": [
            # keep sorted
            {"name": "sa8155-adp-air-overlay.dtbo"},
            {"name": "sa8155-adp-star-overlay.dtbo"},
            {"name": "sa8155p-adp-air-overlay.dtbo"},
            {"name": "sa8155p-adp-star-overlay.dtbo"},
            {"name": "sa8195p-adp-air-overlay.dtbo"},
            {"name": "sa8195p-adp-star-overlay.dtbo"},
        ],
    },
    "gen4auto": {
        "dtb_list": [
            # keep sorted
            {"name": "monaco_auto.dtb"},
            {"name": "monaco_auto-adas.dtb"},
            {"name": "monaco_auto-ivi.dtb"},
        ],
        "dtbo_list": [
            # keep sorted
            {"name": "monaco_auto-adas-adp-air-overlay.dtbo"},
            {"name": "monaco_auto-adas-adp-star-overlay.dtbo"},
            {"name": "monaco_auto-ivi-adp-air-overlay.dtbo"},
            {"name": "monaco_auto-ivi-adp-star-overlay.dtbo"},
            {"name": "monaco_auto-rumi-overlay.dtbo"},
        ],
    },
    "mdm9607": {
        "dtb_list": [
            # keep sorted
            {"name": "mdm9607-cdp.dtb"},
            {"name": "mdm9607-mtp.dtb"},
            {"name": "mdm9607-rcm.dtb"},
        ],
        "dtbo_list": [
            # keep sorted
        ],
    },
    "sdmsteppeauto": {
        "dtb_list": [
            #keep sorted
            {"name": "sa6155.dtb"},
            {"name": "sa6155p.dtb"},
        ],
        "dtbo_list": [
            #keep sorted
            {"name": "sa6155-adp-air-overlay.dtbo"},
            {"name": "sa6155-adp-star-overlay.dtbo"},
            {"name": "sa6155p-adp-air-overlay.dtbo"},
            {"name": "sa6155p-adp-star-overlay.dtbo"},
        ],
    },
    "kalama": {
        "dtb_list": [
            {"name": "kalama.dtb"},
            {"name": "kalama-v2.dtb"},
            {
                "name": "kalamap.dtb",
                "apq": True,
            },
            {
                "name": "kalamap-v2.dtb",
                "apq": True,
            },
        ],
        "dtbo_list": [
            {"name": "kalama-atp-overlay.dtbo"},
            {"name": "kalama-cdp-nfc-overlay.dtbo"},
            {"name": "kalama-cdp-overlay.dtbo"},
            {"name": "kalama-cdp-wsa883x-overlay.dtbo"},
            {"name": "kalama-mtp-nfc-overlay.dtbo"},
            {"name": "kalama-mtp-overlay.dtbo"},
            {"name": "kalama-qrd-overlay.dtbo"},
            {"name": "kalama-rcm-overlay.dtbo"},
            {
                "name": "kalama-rumi-overlay.dtbo",
                "apq": False,
            },
        ],
    },
    "kalama-tuivm": {
        "dtb_list": [
            {"name": "kalama-oemvm-cdp.dtb"},
            {"name": "kalama-oemvm-mtp.dtb"},
            {"name": "kalama-oemvm-qrd.dtb"},
            {"name": "kalama-oemvm-rumi.dtb"},
            {"name": "kalama-vm-cdp.dtb"},
            {"name": "kalama-vm-mtp.dtb"},
            {"name": "kalama-vm-qrd.dtb"},
            {"name": "kalama-vm-rumi.dtb"},
        ],
    },
    "kalama-oemvm": {
        "dtb_list": [
            {"name": "kalama-oemvm-cdp.dtb"},
            {"name": "kalama-oemvm-mtp.dtb"},
            {"name": "kalama-oemvm-qrd.dtb"},
            {"name": "kalama-oemvm-rumi.dtb"},
            {"name": "kalama-vm-cdp.dtb"},
            {"name": "kalama-vm-mtp.dtb"},
            {"name": "kalama-vm-qrd.dtb"},
            {"name": "kalama-vm-rumi.dtb"},
        ],
    },
    "pineapple": {
        "dtb_list": [
            {"name": "pineapple.dtb"},
            {"name": "pineapple-v2.dtb"},
            {
                "name": "pineapplep.dtb",
                "apq": True,
            },
            {
                "name": "pineapplep-v2.dtb",
                "apq": True,
            },
            {
                "name": "pineapplep-sg-v2.dtb",
                "apq": True,
            },
        ],
        "dtbo_list": [
            {"name": "pineapple-atp-overlay.dtbo"},
#            {"name": "pineapple-cdp-nfc-overlay.dtbo"},
#            {"name": "pineapple-cdp-overlay.dtbo"},
#            {"name": "pineapple-mtp-nfc-overlay.dtbo"},
#            {"name": "pineapple-mtp-overlay.dtbo"},
            {"name": "pineapple-qrd-overlay.dtbo"},
            {"name": "pineapple-qrd-sku2-overlay.dtbo"},
            {"name": "pineapple-rcm-overlay.dtbo"},
            {"name": "pineapplep-hdk-overlay.dtbo"},
            {"name": "pineapple-dpm-overlay.dtbo"},
            {"name": "pineapplep-aim500-overlay.dtbo"},
            {"name": "pineapplep-aim500-v2-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-mtp-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-atp-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-cdp-nfc-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-cdp-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-mtp-nfc-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-qrd-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-qrd-sku2-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-rcm-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-aim500-overlay.dtbo"},
            {"name": "pineapplep-sg-v2-hdk-overlay.dtbo"},
        ],
        "binary_compatible_with": ["cliffs", "volcano"],
    },
    "neo-la": {
        "dtb_list": [
            {"name": "neo_la.dtb"},
            {"name": "neo_luna-v2.dtb"},
        ],
        "dtbo_list": [
            {"name": "neo_la-rumi-overlay.dtbo"},
            {"name": "neo-sg-idp-hmt-overlay.dtbo"},
            {"name": "neo-sg-idp-pm8008-overlay.dtbo"},
            {"name": "neo_luna-v2-sg-idp-dual-display-camera-overlay.dtbo"},
            {"name": "neo_luna-v2-sg-idp-dual-display-overlay.dtbo"},
            {"name": "neo_luna-v2-sg-idp-overlay.dtbo"},
            {"name": "neo_luna-v2-sg-idp-pm8008-overlay.dtbo"},
        ],
    },
    "anorak": {
        "dtb_list": [
            {"name": "anorak.dtb"},
            #TODO: Need to add anorakp
            #{
            #    "name": "anorakp.dtb",
            #    "apq": True,
            #},
        ],
        "dtbo_list": [
            {"name": "anorak-idp-overlay.dtbo"},
            {"name": "anorak-atp-overlay.dtbo"},
            {"name": "anorak-qxr-overlay.dtbo"},
            {"name": "anorak-idp-4kdisp-hmt-overlay.dtbo"},
            {"name": "anorak-idp-4kdisp-hsp-overlay.dtbo"},
            {"name": "anorak-idp-hmt-overlay.dtbo"},
            {"name": "anorak-idp-hsp-nodisplay-overlay.dtbo"},
            {"name": "anorak-idp-sd-overlay.dtbo"},
            {"name": "anorak-idp-top-hmt-overlay.dtbo"},
            {"name": "anorak-idp-top-hsp-overlay.dtbo"},
            {"name": "anorak-qxr-4kdisp-hmt-ufs-ref-overlay.dtbo"},
            {"name": "anorak-qxr-hmt-overlay.dtbo"},
            {"name": "anorak-qxr-hmt-ufs-ref-overlay.dtbo"},
        ],
    },
    "niobe": {
        "dtb_list": [
            {"name": "niobe.dtb"},
            {
                "name": "niobep.dtb",
                "apq": True,
            },
        ],
        "dtbo_list": [
            {"name": "niobe-atp-overlay.dtbo"},
            {"name": "niobe-idp-overlay.dtbo"},
            {"name": "niobe-idp-dpu-overlay.dtbo"},
            {"name": "niobe-qxr-overlay.dtbo"},
            {"name": "niobep-atp-overlay.dtbo"},
            {"name": "niobep-idp-overlay.dtbo"},
            {"name": "niobep-idp-dpu-overlay.dtbo"},
            {"name": "niobep-qxr-overlay.dtbo"},
            {
                "name": "niobe-rumi-overlay.dtbo",
                "apq": False,
            },
        ],
    },
    "seraph": {
        "dtb_list": [
            {"name": "seraph.dtb"},
        ],
        "dtbo_list": [
            {
                "name": "seraph-rumi-overlay.dtbo",
            },
        ],
    },
    "cliffs": {
        "dtb_list": [
            {"name": "cliffs.dtb"},
            {"name": "cliffs7.dtb"},
            {
                "name": "cliffsp.dtb",
                "apq": True,
            },
            {
                "name": "cliffs7p.dtb",
                "apq": True,
            },
        ],
        "dtbo_list": [
            {"name": "cliffs-atp-overlay.dtbo"},
            {"name": "cliffs-cdp-overlay.dtbo"},
            {"name": "cliffs-mtp-overlay.dtbo"},
            {"name": "cliffs-qrd-overlay.dtbo"},
            {"name": "cliffs-rcm-overlay.dtbo"},
            {"name": "cliffs-mtp-kiwi-2s-nfc-wcd9395-overlay.dtbo"},
            {"name": "cliffs-mtp-peach-2s-nfc-wcd9395-overlay.dtbo"},
            {"name": "cliffs-mtp-pm8550b-overlay.dtbo"},
            {"name": "cliffs-mtp-kiwi-2s-nfc-wcd9395-pm8550b-overlay.dtbo"},
            {"name": "cliffs-mtp-peach-2s-nfc-wcd9395-pm8550b-overlay.dtbo"},
            {"name": "cliffs-rcm-qhdp-overlay.dtbo"},
            {"name": "cliffs-rcm-fhdp-kiwi-overlay.dtbo"},
            {"name": "cliffs-rcm-qhdp-kiwi-overlay.dtbo"},
            {"name": "cliffs-mtp-kiwi-overlay.dtbo"},
            {"name": "cliffs-mtp-peach-overlay.dtbo"},
        ],
    },
    "pineapple-tuivm": {
        "dtb_list": [
            {"name": "pineapple-oemvm-rcm.dtb"},
            {"name": "pineapple-oemvm-cdp.dtb"},
            {"name": "pineapple-oemvm-mtp.dtb"},
            {"name": "pineapple-oemvm-qrd.dtb"},
            {"name": "pineapplep-oemvm-hdk.dtb"},
            {"name": "pineapple-vm-rcm.dtb"},
            {"name": "pineapple-vm-cdp.dtb"},
            {"name": "pineapple-vm-mtp.dtb"},
            {"name": "pineapple-vm-qrd.dtb"},
            {"name": "pineapplep-vm-hdk.dtb"},
        ],
        "binary_compatible_with": ["cliffs-tuivm", "volcano-tuivm"],
    },
    "pineapple-oemvm": {
        "dtb_list": [
            {"name": "pineapple-oemvm-rcm.dtb"},
            {"name": "pineapple-oemvm-cdp.dtb"},
            {"name": "pineapple-oemvm-mtp.dtb"},
            {"name": "pineapple-oemvm-qrd.dtb"},
            {"name": "pineapplep-oemvm-hdk.dtb"},
            {"name": "pineapple-vm-rcm.dtb"},
            {"name": "pineapple-vm-cdp.dtb"},
            {"name": "pineapple-vm-mtp.dtb"},
            {"name": "pineapple-vm-qrd.dtb"},
            {"name": "pineapplep-vm-hdk.dtb"},
        ],
        "binary_compatible_with": ["cliffs-oemvm", "volcano-oemvm"],
    },
    "niobe-tuivm": {
        "dtb_list": [
            {"name": "niobe-vm-idp.dtb"},
            {"name": "niobep-vm-idp.dtb"},
        ],
    },
    "cliffs-tuivm": {
        "dtb_list": [
            {"name": "cliffs-vm-atp.dtb"},
            {"name": "cliffs-vm-cdp.dtb"},
            {"name": "cliffs-vm-mtp.dtb"},
            {"name": "cliffs-vm-mtp-kiwi.dtb"},
            {"name": "cliffs-vm-mtp-peach.dtb"},
            {"name": "cliffs-vm-qrd.dtb"},
            {"name": "cliffs-vm-rcm.dtb"},
            {"name": "cliffs-vm-rcm-fhdp-kiwi.dtb"},
            {"name": "cliffs-vm-rcm-qhdp.dtb"},
            {"name": "cliffs-vm-rcm-qhdp-kiwi.dtb"},
            {"name": "cliffs-vm-mtp-kiwi-2s-nfc-wcd9395.dtb"},
            {"name": "cliffs-vm-mtp-peach-2s-nfc-wcd9395.dtb"},
            {"name": "cliffs-oemvm-atp.dtb"},
            {"name": "cliffs-oemvm-cdp.dtb"},
            {"name": "cliffs-oemvm-mtp.dtb"},
            {"name": "cliffs-oemvm-mtp-kiwi.dtb"},
            {"name": "cliffs-oemvm-mtp-peach.dtb"},
            {"name": "cliffs-oemvm-qrd.dtb"},
            {"name": "cliffs-oemvm-rcm.dtb"},
            {"name": "cliffs-oemvm-rcm-fhdp-kiwi.dtb"},
            {"name": "cliffs-oemvm-rcm-qhdp.dtb"},
            {"name": "cliffs-oemvm-rcm-qhdp-kiwi.dtb"},
            {"name": "cliffs-oemvm-mtp-kiwi-2s-nfc-wcd9395.dtb"},
            {"name": "cliffs-oemvm-mtp-peach-2s-nfc-wcd9395.dtb"},
        ],
    },
    "cliffs-oemvm": {
        "dtb_list": [
            {"name": "cliffs-vm-atp.dtb"},
            {"name": "cliffs-vm-cdp.dtb"},
            {"name": "cliffs-vm-mtp.dtb"},
            {"name": "cliffs-vm-mtp-kiwi.dtb"},
            {"name": "cliffs-vm-mtp-peach.dtb"},
            {"name": "cliffs-vm-qrd.dtb"},
            {"name": "cliffs-vm-rcm.dtb"},
            {"name": "cliffs-vm-rcm-fhdp-kiwi.dtb"},
            {"name": "cliffs-vm-rcm-qhdp.dtb"},
            {"name": "cliffs-vm-rcm-qhdp-kiwi.dtb"},
            {"name": "cliffs-vm-mtp-kiwi-2s-nfc-wcd9395.dtb"},
            {"name": "cliffs-vm-mtp-peach-2s-nfc-wcd9395.dtb"},
            {"name": "cliffs-oemvm-atp.dtb"},
            {"name": "cliffs-oemvm-cdp.dtb"},
            {"name": "cliffs-oemvm-mtp.dtb"},
            {"name": "cliffs-oemvm-mtp-kiwi.dtb"},
            {"name": "cliffs-oemvm-mtp-peach.dtb"},
            {"name": "cliffs-oemvm-qrd.dtb"},
            {"name": "cliffs-oemvm-rcm.dtb"},
            {"name": "cliffs-oemvm-rcm-fhdp-kiwi.dtb"},
            {"name": "cliffs-oemvm-rcm-qhdp.dtb"},
            {"name": "cliffs-oemvm-rcm-qhdp-kiwi.dtb"},
            {"name": "cliffs-oemvm-mtp-kiwi-2s-nfc-wcd9395.dtb"},
            {"name": "cliffs-oemvm-mtp-peach-2s-nfc-wcd9395.dtb"},
        ],
    },
    "volcano-tuivm": {
        "dtb_list": [
            {"name": "volcano-vm-atp.dtb"},
            {"name": "volcano-vm-idp.dtb"},
            {"name": "volcano-vm-idp-wcd9395.dtb"},
            {"name": "volcano6i-vm-idp-wcd9395-brahma.dtb"},
            {"name": "volcano6i-vm-idp-wcd9395-ganges.dtb"},
            {"name": "volcano-vm-mtp.dtb"},
            {"name": "volcano-vm-mtp-wcd9395-aatc.dtb"},
            {"name": "volcano6i-vm-mtp-brahma.dtb"},
            {"name": "volcano6i-vm-mtp-ganges.dtb"},
            {"name": "volcano6i-vm-mtp-moselle-wingmate.dtb"},
            {"name": "volcano6i-vm-mtp-wcd9395-brahma.dtb"},
            {"name": "volcano6i-vm-mtp-wcd9395-ganges-hac2019.dtb"},
            {"name": "volcano6i-vm-mtp-wcd9395-ganges.dtb"},
            {"name": "volcano6i-vm-mtp-wcd9395-moselle.dtb"},
            {"name": "volcano-vm-qrd.dtb"},
            {"name": "volcano6i-vm-qrd-moselle.dtb"},
            {"name": "volcano-oemvm-atp.dtb"},
            {"name": "volcano-oemvm-idp.dtb"},
            {"name": "volcano-oemvm-idp-wcd9395.dtb"},
            {"name": "volcano6i-oemvm-idp-wcd9395-brahma.dtb"},
            {"name": "volcano6i-oemvm-idp-wcd9395-ganges.dtb"},
            {"name": "volcano-oemvm-mtp.dtb"},
            {"name": "volcano-oemvm-mtp-wcd9395-aatc.dtb"},
            {"name": "volcano6i-oemvm-mtp-brahma.dtb"},
            {"name": "volcano6i-oemvm-mtp-ganges.dtb"},
            {"name": "volcano6i-oemvm-mtp-moselle-wingmate.dtb"},
            {"name": "volcano6i-oemvm-mtp-wcd9395-brahma.dtb"},
            {"name": "volcano6i-oemvm-mtp-wcd9395-ganges-hac2019.dtb"},
            {"name": "volcano6i-oemvm-mtp-wcd9395-ganges.dtb"},
            {"name": "volcano6i-oemvm-mtp-wcd9395-moselle.dtb"},
            {"name": "volcano-oemvm-qrd.dtb"},
            {"name": "volcano6i-oemvm-qrd-moselle.dtb"},
        ],
    },
    "volcano-oemvm": {
        "dtb_list": [
            {"name": "volcano-oemvm-atp.dtb"},
            {"name": "volcano-oemvm-idp.dtb"},
            {"name": "volcano-oemvm-idp-wcd9395.dtb"},
            {"name": "volcano6i-oemvm-idp-wcd9395-brahma.dtb"},
            {"name": "volcano6i-oemvm-idp-wcd9395-ganges.dtb"},
            {"name": "volcano-oemvm-mtp.dtb"},
            {"name": "volcano-oemvm-mtp-wcd9395-aatc.dtb"},
            {"name": "volcano6i-oemvm-mtp-brahma.dtb"},
            {"name": "volcano6i-oemvm-mtp-ganges.dtb"},
            {"name": "volcano6i-oemvm-mtp-moselle-wingmate.dtb"},
            {"name": "volcano6i-oemvm-mtp-wcd9395-brahma.dtb"},
            {"name": "volcano6i-oemvm-mtp-wcd9395-ganges-hac2019.dtb"},
            {"name": "volcano6i-oemvm-mtp-wcd9395-ganges.dtb"},
            {"name": "volcano6i-oemvm-mtp-wcd9395-moselle.dtb"},
            {"name": "volcano-oemvm-qrd.dtb"},
            {"name": "volcano6i-oemvm-qrd-moselle.dtb"},
            {"name": "volcano-vm-atp.dtb"},
            {"name": "volcano-vm-idp.dtb"},
            {"name": "volcano-vm-idp-wcd9395.dtb"},
            {"name": "volcano6i-vm-idp-wcd9395-brahma.dtb"},
            {"name": "volcano6i-vm-idp-wcd9395-ganges.dtb"},
            {"name": "volcano-vm-mtp.dtb"},
            {"name": "volcano-vm-mtp-wcd9395-aatc.dtb"},
            {"name": "volcano6i-vm-mtp-brahma.dtb"},
            {"name": "volcano6i-vm-mtp-ganges.dtb"},
            {"name": "volcano6i-vm-mtp-moselle-wingmate.dtb"},
            {"name": "volcano6i-vm-mtp-wcd9395-brahma.dtb"},
            {"name": "volcano6i-vm-mtp-wcd9395-ganges-hac2019.dtb"},
            {"name": "volcano6i-vm-mtp-wcd9395-ganges.dtb"},
            {"name": "volcano6i-vm-mtp-wcd9395-moselle.dtb"},
            {"name": "volcano-vm-qrd.dtb"},
            {"name": "volcano6i-vm-qrd-moselle.dtb"},
        ],
    },
    "blair": {
        "dtb_list": [
            {"name": "blair.dtb"},
            {"name": "holi.dtb"},
            {
                "name": "blairp.dtb",
                "apq": True,
            },
        ],
        "dtbo_list": [
            {"name": "blair-mtp-overlay.dtbo"},
            {"name": "blair-qrd-overlay.dtbo"},
            {"name": "blair-cdp-overlay.dtbo"},
            {"name": "blair-atp-overlay.dtbo"},
            {"name": "blair-mtp-usbc-overlay.dtbo"},
            {"name": "blair-mtp-nopmi-overlay.dtbo"},
            {"name": "blair-qrd-nopmi-overlay.dtbo"},
            {"name": "blairp-mtp-overlay.dtbo"},
            {"name": "blairp-qrd-overlay.dtbo"},
            {"name": "blairp-cdp-overlay.dtbo"},
            {"name": "blairp-atp-overlay.dtbo"},
            {"name": "blairp-mtp-usbc-overlay.dtbo"},
            {"name": "blairp-mtp-nopmi-overlay.dtbo"},
            {"name": "blairp-qrd-nopmi-overlay.dtbo"},
            {"name": "holi-mtp-overlay.dtbo"},
            {"name": "holi-qrd-overlay.dtbo"},
            {"name": "holi-cdp-overlay.dtbo"},
            {"name": "holi-cdp-lcd-overlay.dtbo"},
            {"name": "holi-atp-overlay.dtbo"},
            {"name": "holi-mtp-usbc-overlay.dtbo"},
            {"name": "holi-mtp-pm6125-overlay.dtbo"},
            {"name": "holi-cdp-pm6125-overlay.dtbo"},
            {"name": "holi-mtp-usbc-pm6125-overlay.dtbo"},
            {"name": "holi-mtp-pm6125-nopmi-overlay.dtbo"},
            {"name": "holi-qrd-pm6125-overlay.dtbo"},
            {"name": "holi-cdp-lcd-pm6125-overlay.dtbo"},
            {"name": "holi-qrd-pm6125-nopmi-overlay.dtbo"},
        ],
    },
    "pitti": {
        "dtb_list": [
            {"name": "pitti.dtb"},
            {"name": "pitti-3gb.dtb"},
            {"name": "pitti-4gb.dtb"},
        ],
        "dtbo_list": [
            {"name": "pitti-atp-overlay.dtbo"},
            {"name": "pitti-idp-overlay.dtbo"},
            {"name": "pitti-qrd-overlay.dtbo"},
            {"name": "pitti-idp-apache-overlay.dtbo"},
            {"name": "pitti-idp-pmiv0104-apache-overlay.dtbo"},
            {"name": "pitti-idp-amoled-overlay.dtbo"},
            {"name": "pitti-idp-pmiv0104-overlay.dtbo"},
            {"name": "pitti-idp-3gb-overlay.dtbo"},
            {"name": "pitti-idp-pmiv0104-3gb-overlay.dtbo"},
            {"name": "pitti-qrd-3gb-overlay.dtbo"},
            {"name": "pitti-atp-4gb-overlay.dtbo"},
            {"name": "pitti-idp-pmiv0104-4gb-overlay.dtbo"},
            {"name": "pitti-idp-4gb-overlay.dtbo"},
            {"name": "pitti-qrd-4gb-overlay.dtbo"},
            {"name": "pitti-idp-90fps-overlay.dtbo"},
            {"name": "pitti-qrd-90fps-overlay.dtbo"},
            {"name": "pitti-idp-pmiv0104-90fps-overlay.dtbo"},
            {"name": "pitti-idp-pmiv0104-amoled-overlay.dtbo"},
            {
                "name": "pitti-rumi-overlay.dtbo",
                "apq": False,
            },
        ],
    },
    "volcano": {
        "dtb_list": [
            {"name": "volcano.dtb"},
            {"name": "volcano6.dtb"},
            {"name": "volcano6i-fp1.dtb"},
            {"name": "volcano6i-fp2.dtb"},
            {"name": "volcano6i-fp3.dtb"},
            {"name": "volcano6i-fp4.dtb"},
            {
                "name": "volcano6p.dtb",
                "apq": True,
            },
            {
                "name": "volcano6ip-fp1.dtb",
                "apq": True,
            },
            {
                "name": "volcano6ip-fp2.dtb",
                "apq": True,
            },
            {
                "name": "volcano6ip-fp3.dtb",
                "apq": True,
            },
            {
                "name": "volcano6ip-fp4.dtb",
                "apq": True,
            },
        ],
        "dtbo_list": [
            {"name": "volcano-atp-overlay.dtbo"},
            {"name": "volcano-idp-overlay.dtbo"},
            {"name": "volcano-idp-wcd9395-overlay.dtbo"},
            {"name": "volcano6i-idp-wcd9395-ganges-overlay.dtbo"},
            {"name": "volcano6i-idp-wcd9395-brahma-overlay.dtbo"},
            {"name": "volcano-mtp-overlay.dtbo"},
            {"name": "volcano-mtp-wcd9395-aatc-overlay.dtbo"},
            {"name": "volcano6i-mtp-wcd9395-ganges-overlay.dtbo"},
            {"name": "volcano6i-mtp-ganges-overlay.dtbo"},
            {"name": "volcano6i-mtp-wcd9395-ganges-hac2019-overlay.dtbo"},
            {"name": "volcano6i-mtp-wcd9395-brahma-overlay.dtbo"},
            {"name": "volcano6i-mtp-brahma-overlay.dtbo"},
            {"name": "volcano6i-mtp-wcd9395-moselle-overlay.dtbo"},
            {"name": "volcano6i-mtp-moselle-wingmate-overlay.dtbo"},
            {"name": "volcano-qrd-overlay.dtbo"},
            {"name": "volcano-qrd-pro-overlay.dtbo"},
            {"name": "volcano6i-qrd-moselle-overlay.dtbo"},
        ],
    },
}

def _get_dtb_lists(target, dt_overlay_supported):
    if not target in _platform_map:
        fail("{} not in device tree platform map!".format(target))

    ret = {
        "dtb_list": [],
        "dtbo_list": [],
    }

    for dtb_node in [target] + _platform_map[target].get("binary_compatible_with", []):
        ret["dtb_list"].extend(_platform_map[dtb_node].get("dtb_list", []))
        if dt_overlay_supported:
            ret["dtbo_list"].extend(_platform_map[dtb_node].get("dtbo_list", []))
        else:
            # Translate the dtbo list into dtbs we can append to main dtb_list
            for dtb in _platform_map[dtb_node].get("dtb_list", []):
                dtb_base = dtb["name"].replace(".dtb", "")
                for dtbo in _platform_map[dtb_node].get("dtbo_list", []):
                    if not dtbo.get("apq", True) and dtb.get("apq", False):
                        continue

                    dtbo_base = dtbo["name"].replace(".dtbo", "")
                    ret["dtb_list"].append({"name": "{}-{}.dtb".format(dtb_base, dtbo_base)})

    return ret

def get_dtb_list(target, dt_overlay_supported = True):
    return [dtb["name"] for dtb in _get_dtb_lists(target, dt_overlay_supported).get("dtb_list", [])]

def get_dtbo_list(target, dt_overlay_supported = True):
    return [dtb["name"] for dtb in _get_dtb_lists(target, dt_overlay_supported).get("dtbo_list", [])]
