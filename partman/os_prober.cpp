// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/os_prober.h"

#include "base/command.h"
#include "base/file_util.h"

namespace installer {

namespace {

// Cache output of `os-prober` command.
QString ReadOsProberOutput() {
  const QString cache_path("/tmp/deepin-installer-os-prober.conf");
  if (QFile::exists(cache_path)) {
    return ReadFile(cache_path);
  } else {
    // Create ignore_uefi file to make os-prober scan windows systems
    // in UEFI mode.
    const QString partman_flag = "/var/lib/partman/ignore_uefi";
    if (!CreateParentDirs(partman_flag)) {
      qWarning() << "Failed to create parent folder of: " << partman_flag;
    }
    WriteTextFile(partman_flag, "deepin-installer");

    QString output;
    if (SpawnCmd("os-prober", {}, output)) {
      WriteTextFile(cache_path, output);
      return output;
    } else {
      qCritical() << "os-prober failed";
      return QString();
    }
  }
}

}  // namespace

OsProberItems GetOsProberItems() {
  OsProberItems result;

  const QString output = ReadOsProberOutput();
  if (!output.isEmpty()) {
    for (const QString& line : output.split('\n')) {
      if (line.isEmpty()) {
        continue;
      }

      const QStringList items = line.split(':');
      Q_ASSERT(items.length() == 4);
      if (items.length() != 4) {
        continue;
      }

      // Removed version name from description.
      QString description = items.at(1);
      const int left_bracket_index = description.indexOf('(');
      if (left_bracket_index > -1) {
        description = description.left(left_bracket_index).trimmed();
      }
      const QString distro_name = items.at(2);

      OsType type;
      if (line.contains("linux", Qt::CaseInsensitive)) {
        type = OsType::Linux;
      } else if (line.contains("windows", Qt::CaseInsensitive)) {
        type = OsType::Windows;
      } else if (line.contains("mac", Qt::CaseInsensitive)) {
        // FIXME(xushaohua): Check macOS efi entry.
        type = OsType::Mac;
      } else {
        type = OsType::Unknown;
      }

      // Deal with EFI path, like "/dev/sda2@/EFI/Microsoft/Boot/bootmgfw.efi.
      const QString dev_path = items[0];
      const int index = dev_path.indexOf('@');
      const QString path = (index == -1) ? dev_path : dev_path.left(index);

      const OsProberItem prober_item = {path, description, distro_name, type};
      result.append(prober_item);
    }
  }

  return result;
}

}  // namespace installer